#include"ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager(const size_t& work_thread_count, const size_t &output_thread_count, size_t& max_queue_size)
	: taskPool(new ThreadPool(work_thread_count, max_queue_size)), resultPool(new ThreadPool(output_thread_count))
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
	result += "currently working thread count : " + std::to_string(taskPool->getCurrentWorkingThread()) + '\n';
	result += "queued task count : " + std::to_string(taskPool->getQueuedTaskCount()) + '\n';
	result += "=======================================\n";
	result += "Result Pool (thread size : " + std::to_string(resultPool->getThreadCount()) + ", queue size : " + std::to_string(resultPool->getQueueSize()) + ")\n";
	result += "---------------------------------------\n";
	result += "currently working thread count : " + std::to_string(resultPool->getCurrentWorkingThread()) + '\n';
	result += "queued task count : " + std::to_string( resultPool->getQueuedTaskCount() )+ '\n';
	result += "***************************************\n\n";

	return result;
}

void ThreadPoolManager::AddTask(Calc* c) {
	
	auto workAndResult = [c, this] {
		c->execute();
		resultPool->insertTask([c] {c->callback(); });
	};

	taskPool->insertTask(workAndResult);
}

void ThreadPoolManager::StopForTestEnd() {
	taskPool->restartWorkers();
}

void ThreadPoolManager::ForceQuitTest() {
	taskPool->forceRestartWorkers();
}