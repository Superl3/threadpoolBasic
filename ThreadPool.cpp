#include "ThreadPool.h"

ThreadPool::ThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
	if (worker_thread_count > 0)
		for (size_t i = 0; i < worker_thread_count; ++i)
			worker_threads.emplace_back([this] { this->work(); });
}
ThreadPool::~ThreadPool() {
	stop_all = true;
	task_buffer_cv.notify_all();

	for (auto& t : worker_threads)
		t.join();

	worker_threads.clear();
}

#include<iostream>
void ThreadPool::work() {
	while (true) {

		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(task_buffer_mutex);
			task_buffer_cv.wait(lock, [this] { return !this->task_buffer.empty() || stop_all; });
			if (stop_all) break;
			task = std::move(task_buffer.front());
			task_buffer.pop_front();
		}
		task();
	}
}

int ThreadPool::getAvailableCount() {
	return -1;
}
int ThreadPool::getQueuedTaskCount() {
	return task_buffer.size();
}

bool ThreadPool::insertTask(std::function<void()> f) {

	bool isInserted = false;

	if (stop_all) {
	}
	else {
		std::lock_guard<std::mutex> lock(task_buffer_mutex);
		if (task_buffer.size() < max_queue_size || max_queue_size == -1) {
			task_buffer.emplace_back(f);
			isInserted = true;
		}
		task_buffer_cv.notify_one();
	}
	return isInserted;
}
