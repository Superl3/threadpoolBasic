#pragma once

#include "ThreadPool.h"
#include "WorkThreadPool.h"
#include "Calc.h"

class ThreadPoolManager {
public:
	ThreadPoolManager(const int& work_thread_count, const int& output_thread_count, int& max_queue_size);
	~ThreadPoolManager();

	bool AddTask(Calc* c);
	void StopForTestEnd();
	void ForceQuitTest();
	bool isDisabled() { return getWorkThreadCount() == -1; }

	std::string getTPMinfo();

	int getWorkThreadCount() { return taskPool->getThreadCount(); }

private:
	WorkThreadPool *taskPool = nullptr;
	ThreadPool *resultPool = nullptr;
};