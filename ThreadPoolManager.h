#pragma once

#include "ThreadPool.h"

class ThreadPoolManager {
private:
	ThreadPool taskPool;
	ThreadPool resultPool;
public:
	ThreadPoolManager(const size_t& num_threads, size_t output_threads = 3);

	template <class F, class... Args>
	void AddTask(F&& f, Args&&... args,
		std::function<void(std::invoke_result_t<F(Args...)>)> callback = nullptr);

	/*template <class F, class... Args>
	void AddTask(F&& f, Args&&... args);*/

};

#include "ThreadPoolManager.hpp"