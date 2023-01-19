//#include<iostream>
//
//template <class F, class... Args>
//void ThreadPoolManager::AddTask(F&& f, Args&&... args)
//{
//	auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
//	taskPool.EnqueueJob([&task] { task; });
//	return;
//}
