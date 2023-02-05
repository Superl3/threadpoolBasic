#include "WorkThreadPool.h"
#include <iostream>

WorkThreadPool::WorkThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
	if (worker_thread_count > 0) {
		for (size_t i = 0; i < worker_thread_count; ++i) {
			auto thread = new WorkThread(i, this);
			worker_threads.emplace_back(thread);
			available_threads.push_back(thread);
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
	thread_cv.notify_all();
	thread.join();
}
void WorkThread::work() {
	while (!stop || !task) {
		{
			std::unique_lock<std::mutex> lock(thread_mutex);
			thread_cv.wait(lock, [this] { return task; });
#ifdef BRANDON_DEBUG
			g_worker_notify_count_mutex.lock();
			g_worker_notify_count++;
			if (g_worker_notify_count == g_test_debug_count)
			{
				std::cout << "Work Notify Received Done\n";
				g_worker_notify_count = 0;
			}
			g_worker_notify_count_mutex.unlock();
#endif
//			std::cout << "<work>";
			if (task != NULL) {
				task();
			}
			task = NULL;
//			std::cout << "<workDone>";
			notifier->InsertAvailableThread(id);
		}
	}
}
void WorkThread::assignTask(std::function<void()> task_) {
	task = task_;
//	std::cout << "<notify_all:" << id << ">";
	thread_cv.notify_all();
}

#include "PerformanceMonitor.h"
#include<iostream>

void WorkThreadPool::ThreadManaging() {

	auto takeAvailableThread = [this]() {
		WorkThread* thread = NULL;
		while (true) {

			//std::lock_guard<std::mutex> lock_for_available_thread(available_thread_mutex);
			//std::cout << "<takelocks>";

			//std::cout << "<takelocke>";
			task_buffer_mutex.lock();
			if (stop_all && task_buffer.empty())
			{
				task_buffer_mutex.unlock();
				//std::cout << "<takelocke>";
				break;
			}
			task_buffer_mutex.unlock();
			available_thread_mutex.lock();
			if (available_threads.empty()) {
				available_thread_mutex.unlock();
				//std::cout << "<takelocke2>";
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			thread = std::move(available_threads.front());
			available_threads.pop_front();
			available_thread_mutex.unlock();
			//std::cout << "<takelocke3>";
			break;
		}
		return thread;
	};
	auto takeTask = [this]() {
		std::function<void()> task = NULL;
		while (true) {
			//std::lock_guard<std::mutex> lock_for_buffer(task_buffer_mutex);
			//std::cout << "2-1";
			task_buffer_mutex.lock();
			//std::cout << "2-2";
			if (task_buffer.empty()) {
				if (stop_all)
				{
					task_buffer_mutex.unlock();
					break;
				}
				task_buffer_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}
			//std::cout << "2-3";
			task = std::move(task_buffer.front());
			task_buffer.pop_front();
			task_buffer_mutex.unlock();
#ifdef BRANDON_DEBUG
			g_task_buffer_count_mutex.lock();
			g_task_buffer_count++;
			if (g_task_buffer_count == g_test_debug_count)
			{
				std::cout << "g_task_buffer_count done\n";
				g_task_buffer_count = 0;
			}
			g_task_buffer_count_mutex.unlock();
#endif
			// pooling task queue
			//std::cout << "<get_task>";
			break;
		}
		return task;
	};

	
	while (true) {
		PerformanceMonitor perf1, perf2;
	//	std::cout << "<111>";
		auto thread = takeAvailableThread();
		//std::cout << "<222>";
		auto task = takeTask();
		//std::cout << "<333>";
		if (stop_all && task == NULL) break;
	//	std::cout << "<444>";
		thread->assignTask(task);
	//	std::cout << "<555>";
#ifdef BRANDON_DEBUG
		g_taskm_buffer_count_mutex.lock();
		g_taskm_buffer_count++;
		if (g_taskm_buffer_count == g_test_debug_count)
		{
			std::cout << "task Manager received Done\n";
			g_taskm_buffer_count = 0;
		}
		g_taskm_buffer_count_mutex.unlock();

#endif
	}

	std::cout << "Worker Thread Pool done\n";
}

int WorkThreadPool::getQueuedTaskCount() {
	return task_buffer.size();
}

void WorkThreadPool::InsertAvailableThread(const size_t &index) {
	//std::lock_guard<std::mutex> lock(available_thread_mutex);
//	std::cout << "<insertlocks>";
	available_thread_mutex.lock();
	available_threads.push_back(worker_threads[index]);
	available_thread_mutex.unlock();
//	std::cout << "<insertlocke>";
}

bool WorkThreadPool::insertTask(std::function<void()> f) {

	bool isInserted = false;

	if (stop_all) {
	}
	else {
		if (task_buffer.size() < max_queue_size || max_queue_size == -1) {
			{
				//std::cout << "4";
				//std::lock_guard<std::mutex> lock(task_buffer_mutex);
				task_buffer_mutex.lock();
				//std::cout << "5";
				task_buffer.emplace_back(f);
				//std::cout << "6";
				task_buffer_mutex.unlock();
				//std::cout << "<it>";
			}
			isInserted = true;
		}
	}
	return isInserted;
}
