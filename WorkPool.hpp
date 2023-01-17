template <class F, class... Args>
void WorkPool::EnqueueJob(F&& f, Args&&... args,
	std::function<void(std::result_of_t<F(Args...)>)> callback) {

	if (stop_all) {
		throw std::runtime_error("ThreadPool »ç¿ë ÁßÁöµÊ");
	}
	std::time.now()
		auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
	auto taskAndResult = [task, callback] {
		auto result = task();
		if (callback) callback(result);
	};
	{
		std::lock_guard<std::mutex> lock(taskContainerMutex);
		Tasks.emplace_back(taskAndResult);
	}
	taskContainerCV.notify_one();
	return;
}