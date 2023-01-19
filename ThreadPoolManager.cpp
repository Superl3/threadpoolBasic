#include"ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager(const size_t& num_threads, size_t output_threads)
	: taskPool(num_threads), resultPool(output_threads){
	output_thread = std::thread(&ThreadPoolManager::outputLoop, this);
}

void ThreadPoolManager::AddTask(Calc* c) {

	auto workAndResult = [c, this] {
		c->execute();
		resultPool.EnqueueJob([c] {c->callback(); });
	};

	taskPool.EnqueueJob(workAndResult);
}

void ThreadPoolManager::outputLoop() {
	while (true) {
		bool task_done = false;
		/*
		for (int i = 0; i < output_data.size(); i++) {
			auto status = output_data[i].output.wait_for(std::chrono::milliseconds(1));
			if (status != std::future_status::ready) continue;

			om.process(std::any_cast<calcData>(output_data[i].input), std::any_cast<int>(output_data[i].output.get()), 1);

			output_data.erase(output_data.begin() + i);
			task_done = true;
			break;
		}
		*/
		if (!task_done) continue;

	}
}
