#pragma once
#include "ThreadPool.h"
class WorkPool : ThreadPool {
	// job �� �߰��Ѵ�.
	template <class F, class... Args>
	void EnqueueJob(F&& f, Args&&... args,
		std::function<void(std::result_of_t<F(Args...)>)> callback = nullptr);

};

#include "WorkPool.hpp"