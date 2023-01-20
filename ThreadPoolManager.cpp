#include"ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager(const size_t& work_thread_count, const size_t &output_thread_count, size_t& max_queue_size)
	: taskPool(work_thread_count, max_queue_size), resultPool(output_thread_count)
{
}

void ThreadPoolManager::AddTask(Calc* c) {

	auto workAndResult = [c, this] {
		c->execute();
		resultPool.EnqueueJob([c] {c->callback(); });
	};

	taskPool.EnqueueJob(workAndResult);
}
