#include "WorkThreadPool.h"
#include<iostream>

WorkThreadPool::WorkThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
	if (worker_thread_count > 0) {
		for (size_t i = 0; i < worker_thread_count; ++i) {
			auto thread = new WorkThread(i, this);
			worker_threads.emplace_back(thread);
			available_threads.push_back(i);
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
			std::unique_lock<std::mutex> lock(thread_mutex);
			thread_cv.wait(lock, [this] { return task; });

			g_wait_task_mutex.lock();
			g_wait_task_count += 1;
			if (g_wait_task_count == 100000) {
				std::cout << "wait task done\n";
				g_wait_task_count = 0;
			}
			g_wait_task_mutex.unlock();

			task();
			task = NULL;
			notifier->InsertAvailableThread(id);
		}
	}
}
void WorkThread::assignTask(std::function<void()> task_) {
	task = task_;
	thread_cv.notify_one();
	g_insert_task_mutex.lock();
	g_insert_task_count += 1;
	if (g_insert_task_count == 100000) {
		std::cout << "insert task done\n";
		g_insert_task_count = 0;
	}
	g_insert_task_mutex.unlock();
}

#include "PerformanceMonitor.h"

void WorkThreadPool::ThreadManaging() {

	auto takeAvailableThread = [this]() {
		WorkThread* thread = NULL;
		while (true) {
			std::unique_lock<std::mutex> lock_for_available_thread(available_thread_mutex);
			if (available_threads.empty()) {
				lock_for_available_thread.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
			//task_buffer_cv.wait(lock_for_buffer, [this] { return !task_buffer.empty() || stop_all; });
			if (task_buffer.empty()) {
				if (stop_all) break;
				lock_for_buffer.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			task = std::move(task_buffer.front());
			task_buffer.pop_front();
			// pooling task queue
			break;
		}
		return task;
	};

	
	while (true) {
		//PerformanceMonitor perf1, perf2;
		//perf1.setStartTimer();
		auto thread = takeAvailableThread();
		//perf1.setEndTimer();	
		//perf2.setStartTimer();
		auto task = takeTask();
		//perf2.setEndTimer();
		if (stop_all && task == NULL) break;
		//std::cout << perf1.getRunningTime() << ", " << perf2.getRunningTime() << '\n';
		thread->assignTask(task);
	}
}

int WorkThreadPool::getQueuedTaskCount() {
	return task_buffer.size();
}

void WorkThreadPool::InsertAvailableThread(const size_t &index) {
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
			task_buffer_cv.notify_all();
		}
	}
	return isInserted;
}
