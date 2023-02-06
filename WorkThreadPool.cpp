#include "WorkThreadPool.h"
#include "Utility.h"
#include<iostream>

WorkThreadPool::WorkThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
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
	thread_cv.notify_one();
	thread.join();
}
void WorkThread::work() {
	while (!stop || !task) {
		{
			std::unique_lock<std::mutex> lk(thread_mutex);
			notifier->InsertAvailableThread(id);
			thread_cv.wait(lk, [this] { return task; });
			
			/*while (true) {
				std::unique_lock<std::mutex> lk(thread_mutex);
				if (task) break;
				lk.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}*/
			task();
			task = NULL;
		}
	}
}
#include<string>
void WorkThread::assignTask(std::function<void()> task_) {
	std::lock_guard<std::mutex> lk(thread_mutex);
	task = task_;
	thread_cv.notify_one();
}

#include "PerformanceMonitor.h"

void WorkThreadPool::ThreadManaging() {
	auto takeAvailableThread = [this]() {
		WorkThread* thread = NULL;
		while (true) {
			std::unique_lock<std::mutex> lock_for_available_buffer(available_thread_mutex);
			if (available_threads.empty()) {
				lock_for_available_buffer.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
			std::unique_lock<std::mutex> lock_for_buffer(task_buffer_mutex);
			task_buffer_cv.wait(lock_for_buffer, [this] { return !task_buffer.empty() || stop_all; });
			if (task_buffer.empty()) {
				if (stop_all) break;
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
		if (stop_all && task == NULL) break;
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

int WorkThreadPool::getQueuedTaskCount() {
	return task_buffer.size();
}

void WorkThreadPool::InsertAvailableThread(const size_t& index) {

	//std::string str = std::to_string(index) + "is now available \n";
	//std::cout << str;

	std::lock_guard<std::mutex> lock(available_thread_mutex);
	available_threads.push_back(index);
}

bool WorkThreadPool::insertTask(std::function<void()> f) {

	bool isInserted = false;

	if (stop_all) {
	}
	else {
		if (task_buffer.size() < max_queue_size || max_queue_size == -1) {
			{
				std::lock_guard<std::mutex> lock(task_buffer_mutex);
				task_buffer.emplace_back(f);
			}
			isInserted = true;
			task_buffer_cv.notify_one();
		}
	}
	return isInserted;
}
