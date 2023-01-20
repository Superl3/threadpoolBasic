#pragma once

#include "ThreadPool.h"
#include "Calc.h"

class ThreadPoolManager {
public:
	ThreadPoolManager(const size_t& work_thread_count, const size_t& output_thread_count, size_t& max_queue_size);

	void AddTask(Calc* c);

private:
	ThreadPool taskPool;
	ThreadPool resultPool;
};

#include "ThreadPoolManager.hpp"