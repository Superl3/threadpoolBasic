#include "WorkThreadPool.h"

WorkThreadPool::WorkThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
	createWorkers();
}
WorkThreadPool::~WorkThreadPool() {
	stopWorkers();
}

void WorkThreadPool::stopWorkers() {
	stop_all = true;
	task_buffer_cv.notify_all();

	for (auto& t : worker_threads) {
		t.join();
	}
	worker_threads.clear();
	worker_thread_status.clear();
}

void WorkThreadPool::forceRestartWorkers() {
	std::lock_guard<std::mutex> lock(task_buffer_mutex);
	task_buffer.clear();
}
#include<iostream>
void WorkThreadPool::restartWorkers() {
	stopWorkers();
	stop_all = false;
	createWorkers();
}


void WorkThreadPool::createWorkers() {
	if (worker_thread_count > 0)
		for (size_t i = 0; i < worker_thread_count; ++i)
			worker_threads.emplace_back([this] { this->work(); });
}

#include<iostream>
void WorkThreadPool::work() {
	std::thread::id thread_id = std::this_thread::get_id();
	while (true) {

		worker_thread_status[thread_id] = false;
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(task_buffer_mutex);
			task_buffer_cv.wait(lock, [this] { return !this->task_buffer.empty() || stop_all; });
			if (stop_all && this->task_buffer.empty()) break;
			task = std::move(task_buffer.front());
			task_buffer.pop_front();
		}

		worker_thread_status[thread_id] = true;
		task();
	}
}

int WorkThreadPool::getCurrentWorkingThread() {
	int cnt = 0;
	{
		std::lock_guard<std::mutex> lk(status_mutex);
		for (const auto& onWork : worker_thread_status) {
			if (onWork.second)
				cnt += 1;
		}
	}

	return cnt;
}
int WorkThreadPool::getQueuedTaskCount() {
	return task_buffer.size();
}

bool WorkThreadPool::insertTask(std::function<void()> f) {

	bool isInserted = false;

	if (stop_all) {
		;
	}
	else {
		if (task_buffer.size() < max_queue_size || max_queue_size == -1) {
			{
				std::lock_guard<std::mutex> lock(task_buffer_mutex);
				task_buffer.emplace_back(f);
			}
			task_buffer_cv.notify_one();
			isInserted = true;
		}
		else {
			;
		}
	}
	return isInserted;
}
