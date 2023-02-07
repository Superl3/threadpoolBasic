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

	// Worker �����带 �����ϴ� ����.
	std::deque<std::thread> worker_threads;
	// ���� job ť�� ���� cv �� m.
	std::condition_variable task_buffer_cv;
	std::mutex task_buffer_mutex;

	// Worker ������
	void work();
};