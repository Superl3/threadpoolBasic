#pragma once

#include "ThreadPool.h"
#include "WorkThreadPool.h"
#include "Calc.h"

class ThreadPoolManager {
public:
	ThreadPoolManager(const int& work_thread_count, const int& output_thread_count, int& max_queue_size);
	~ThreadPoolManager();

	bool AddTask(Calc* c);
	void StopForTestEnd(const size_t &test_count);
	bool isDisabled() { return getWorkThreadCount() == -1; }

	std::string getTPMinfo();

	int getWorkThreadCount() { return taskPool->getThreadCount(); }

	void stopTest() { stop_test = true; }

private:
	bool stop_all = false;
	bool stop_test = false;

	WorkThreadPool *taskPool = nullptr;
	WorkThreadPool *resultPool = nullptr;

	std::mutex test_cnt_mutex;
	size_t test_cnt = 0;
};