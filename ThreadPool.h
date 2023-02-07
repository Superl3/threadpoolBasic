#pragma once

#include "BasicThreadPool.h"

#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <map>

class ThreadPool : public BasicThreadPool {
public:
	ThreadPool(const int& num_threads, int queue_size = -1);
	~ThreadPool();

	// return false if queue is full or threadpool done
	bool insertTask(std::function<void()> f) override;

	int getAvailableCount() override;

private:

	// Worker 쓰레드를 보관하는 벡터.
	std::deque<std::thread> worker_threads;
	// 위의 job 큐를 위한 cv 와 m.
	std::condition_variable task_buffer_cv;
	std::mutex task_buffer_mutex;

	// Worker 쓰레드
	void work();
};