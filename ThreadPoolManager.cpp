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

void ThreadPoolManager::AddTask(Calc* c) {
	
	auto workAndResult = [c, this] {
		c->execute();
		resultPool->insertTask([c] {c->callback(); });
	};

	taskPool->insertTask(workAndResult);
}

void ThreadPoolManager::StopForTestEnd() {

}