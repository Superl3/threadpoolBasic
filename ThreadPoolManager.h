#pragma once

#include "ThreadPool.h"
#include "PerformanceMonitor.h"
#include "Output.h"
#include "Calc.h"

#include <vector>

class ThreadPoolManager {
private:

	struct dataSet {	
		PerformanceMonitor performance_monitor;
	};
	void outputLoop();

	ThreadPool taskPool;
	ThreadPool resultPool;

	std::mutex output_mutex;
	std::condition_variable output_cv;
	std::vector<dataSet> output_data;

	std::thread output_thread;

	Output om;
public:
	ThreadPoolManager(const size_t& num_threads, size_t output_threads = 1);

	void AddTask(Calc *c);

	/*template <class F, class... Args>
	void AddTask(F&& f, Args&&... args);*/
};

#include "ThreadPoolManager.hpp"