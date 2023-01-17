//template <class F>
//void ThreadPool::EnqueueJob(F&& f, std::function<void(std::result_of_t<F>)> callback) {
//
//	if (stop_all) {
//		throw std::runtime_error("ThreadPool »ç¿ë ÁßÁöµÊ");
//	}
//	auto task = std::bind(std::forward<F>(f));
//	auto taskAndResult = [task, callback] {
//		auto result = task();
//		if (callback) callback(result);
//	};
//	{
//		std::lock_guard<std::mutex> lock(taskBufferMutex);
//		taskBuffer.emplace_back(taskAndResult);
//	}
//	taskBufferCV.notify_one();
//	return;
//}