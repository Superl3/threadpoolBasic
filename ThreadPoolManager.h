#pragma once

#include "ThreadPool.h"
#include "Calc.h"

class ThreadPoolManager {
public:
	ThreadPoolManager(const size_t& work_thread_count, const size_t& output_thread_count, size_t& max_queue_size);
	~ThreadPoolManager();

	void AddTask(Calc* c);
	void StopForTestEnd();

	size_t getWorkThreadCount() { return taskPool->getThreadCount(); }

private:
	ThreadPool *taskPool = nullptr;
	ThreadPool *resultPool = nullptr;
};