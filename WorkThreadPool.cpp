#include "WorkThreadPool.h"
#include "Utility.h"
#include<iostream>

WorkThreadPool::WorkThreadPool(const int& num_threads, int max_queue_size_)
	: BasicThreadPool(num_threads, max_queue_size_) {
	if (worker_thread_count > 0) {
		for (size_t i = 0; i < worker_thread_count; ++i) {
			auto thread = new WorkThread(i, this);
			worker_threads.emplace_back(thread);
		}
	}

	managing_thread = std::thread(&WorkThreadPool::ThreadManaging, this);
}
WorkThreadPool::~WorkThreadPool() {
	stop_all = true;
	managing_thread.join();

	for (auto& t : worker_threads) {
		delete t;
	}
}

WorkThread::WorkThread(int index, ThreadNotifier* noti) : id(index), notifier(noti) {
	thread = std::thread(&WorkThread::work, this);
}

WorkThread::~WorkThread() {
	stop = true;
#ifndef THREAD_POLLING
	thread_cv.notify_one();
#endif
	thread.join();
}
void WorkThread::work() {
	while (!stop || !task) {
#ifndef THREAD_POLLING
		std::unique_lock<std::mutex> lk(thread_mutex);
		notifier->InsertAvailableThread(id);
		thread_cv.wait(lk, [this] { return task || stop; });
#else
		notifier->InsertAvailableThread(id);
		while (true) {
			std::unique_lock<std::mutex> lk(thread_mutex);
			if (task || stop) break;
			lk.unlock();
			std::this_thread::yield();
		}
#endif
		if (stop) break;
		task();
		task = NULL;
	}
}
void WorkThread::assignTask(std::function<void()> task_) {
	std::lock_guard<std::mutex> lk(thread_mutex);
	task = task_;
#ifndef THREAD_POLLING
	thread_cv.notify_one();
#endif
}

void WorkThreadPool::ThreadManaging() {
	auto takeAvailableThread = [this]() {
		WorkThread* thread = NULL;
		while (true) {
			if (stop_all) break;
			std::unique_lock<std::mutex> lock_for_available_buffer(available_thread_mutex);
			if (available_threads.empty()) {
				lock_for_available_buffer.unlock();
				std::this_thread::yield();
				continue;
			}
			thread = worker_threads[available_threads.front()];
			available_threads.pop_front();
			break;
		}
		return thread;
	};
	auto takeTask = [this]() {
		std::function<void()> task = NULL;
		while (true) {
			if (stop_all) break;
			std::unique_lock<std::mutex> lock_for_buffer(task_buffer_mutex);
			if (task_buffer.empty()) {
				lock_for_buffer.unlock();
				std::this_thread::yield();
				continue;
			}

			task = std::move(task_buffer.front());
			task_buffer.pop_front();
			break;
		}
		return task;
	};


	while (true) {
		auto thread = takeAvailableThread();
		auto task = takeTask();
		if (stop_all) break;
		thread->assignTask(task);
	}
}

int WorkThreadPool::getAvailableCount() {
	int ret = 0;
	available_thread_mutex.lock();
	ret = available_threads.size();
	available_thread_mutex.unlock();
	return ret;
}

void WorkThreadPool::InsertAvailableThread(const size_t& index) {


	std::lock_guard<std::mutex> lock(available_thread_mutex);
	available_threads.push_back(index);
}

bool WorkThreadPool::insertTask(std::function<void()> f) {

	bool isInserted = false;

	if (stop_all) {
	}
	else {
		std::lock_guard<std::mutex> lock(task_buffer_mutex);
		if (task_buffer.size() < max_queue_size || max_queue_size == -1) {
			task_buffer.emplace_back(f);
			isInserted = true;
		}
	}
	return isInserted;
}
