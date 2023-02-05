#include"ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager(const int& work_thread_count, const int&output_thread_count, int& max_queue_size)
	: taskPool(new WorkThreadPool(work_thread_count, max_queue_size)), resultPool(new ThreadPool(output_thread_count))
{
}

ThreadPoolManager::~ThreadPoolManager() {
	delete taskPool;
	taskPool = nullptr;

	delete resultPool;
	resultPool = nullptr;

}

std::string ThreadPoolManager::getTPMinfo() {
	std::string result = "\n";

	result += "ThreadPoolManager Info\n";
	result += "***************************************\n";
	result += "Work Pool (thread size : " + std::to_string(taskPool->getThreadCount()) + ", queue size : " + std::to_string(taskPool->getQueueSize()) + ")\n";
	result += "---------------------------------------\n";
	//result += "currently working thread count : " + std::to_string(taskPool->getCurrentWorkingThread()) + '\n';
	result += "queued task count : " + std::to_string(taskPool->getQueuedTaskCount()) + '\n';
	result += "=======================================\n";
	result += "Result Pool (thread size : " + std::to_string(resultPool->getThreadCount()) + ", queue size : " + std::to_string(resultPool->getQueueSize()) + ")\n";
	result += "---------------------------------------\n";
	result += "currently working thread count : " + std::to_string(resultPool->getCurrentWorkingThread()) + '\n';
	result += "queued task count : " + std::to_string( resultPool->getQueuedTaskCount() )+ '\n';
	result += "***************************************\n\n";

	return result;
}

#include<iostream>
bool ThreadPoolManager::AddTask(Calc* c) {
	
	auto workAndResult = [c, this] {
		auto result = c->execute();
		if (c->isTest()) {
			std::lock_guard<std::mutex> lock(test_cnt_mutex);
			test_cnt += 1;
		}
		resultPool->insertTask([result, c] {c->callback(result); }); //brandon-TBD
	};
	//std::cout << "3";
	return taskPool->insertTask(workAndResult);
}

void ThreadPoolManager::StopForTestEnd(const size_t& test_count) {
	while (test_count > test_cnt) {
		{
			//std::lock_guard<std::mutex> lock(test_cnt_mutex);
			//std::cout << " count : " << test_cnt << "\n";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	test_cnt = 0;
}

void ThreadPoolManager::ForceQuitTest() {
	//taskPool->forceRestartWorkers();
}