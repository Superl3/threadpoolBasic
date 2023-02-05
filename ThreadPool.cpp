#include "ThreadPool.h"

ThreadPool::ThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
	createWorkers();
}
ThreadPool::~ThreadPool() {
	stopWorkers();
}

void ThreadPool::stopWorkers() {
	stop_all = true;
	task_buffer_cv.notify_all();

	for (auto& t : worker_threads) {
		t.join();
	}
	worker_threads.clear();
	worker_thread_status.clear();
}

void ThreadPool::forceRestartWorkers() {
	//std::lock_guard<std::mutex> lock(task_buffer_mutex);
	threadpool_task_buffer_mutex.lock();
	threadpool_task_buffer.clear();
	threadpool_task_buffer_mutex.unlock();
}
#include<iostream>
void ThreadPool::restartWorkers() {
	stopWorkers();
	stop_all = false;
	createWorkers();
}


void ThreadPool::createWorkers() {
	if(worker_thread_count > 0)
		for (size_t i = 0; i < worker_thread_count; ++i) 
			worker_threads.emplace_back([this] { this->work(); });
}

#include<iostream>
void ThreadPool::work() {
	std::thread::id thread_id = std::this_thread::get_id();
	while (true) {

		worker_thread_status[thread_id] = false;
		std::function<void()> task;
		{
			//std::unique_lock<std::mutex> lock(task_buffer_mutex);
			threadpool_task_buffer_mutex.lock();
			//std::cout << "<9>";
			//task_buffer_cv.wait(lock, [this] { return !this->task_buffer.empty() || stop_all; });
			if (stop_all && this->threadpool_task_buffer.empty())
			{
				threadpool_task_buffer_mutex.unlock();
				//std::cout << "9-1";
				break;
			}
			if (this->threadpool_task_buffer.empty())
			{
				threadpool_task_buffer_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				//std::cout << "9-2";
				continue;
			}
			//std::cout << "<9-3>";
			task = std::move(threadpool_task_buffer.front());
			threadpool_task_buffer.pop_front();
			threadpool_task_buffer_mutex.unlock();
			//std::cout << "<9-3-1>";
		}

		worker_thread_status[thread_id] = true;
		//std::cout << "<1>";
		task();
		//std::cout << "<2>";
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//std::cout << "1-1";
	}
}

int ThreadPool::getCurrentWorkingThread() {
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
int ThreadPool::getQueuedTaskCount() {
	return threadpool_task_buffer.size();
}

bool ThreadPool::insertTask(std::function<void()> f) {

	bool isInserted = false;

	if (stop_all) {
		;
	}
	else {
		if (threadpool_task_buffer.size() < max_queue_size || max_queue_size == -1) {
			{
				//std::lock_guard<std::mutex> lock(task_buffer_mutex);
//				std::cout << "<its>";
				threadpool_task_buffer_mutex.lock();
				threadpool_task_buffer.emplace_back(f);
				threadpool_task_buffer_mutex.unlock();
//				std::cout << "<ite>";
			}
			//task_buffer_cv.notify_one();
			isInserted = true;
		}
		else {
			;
		}
	}
	return isInserted;
}
