template <class F, class... Args>
void ThreadPoolManager::AddTask(F&& f, Args&&... args,
	std::function<void(std::invoke_result_t<F(Args...)>)> callback)
{
	auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
	//std::function<std::result_of_t< F(Args...)>()>
	auto taskAndResult = [&task, callback, this] {
		task;
		//callback(task);
		//resultPool.EnqueueJob(resFunc);
	};
	taskPool.EnqueueJob(taskAndResult);
	return;
}

//#include<iostream>
//
//template <class F, class... Args>
//void ThreadPoolManager::AddTask(F&& f, Args&&... args)
//{
//	auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
//	taskPool.EnqueueJob([&task] { task; });
//	return;
//}
