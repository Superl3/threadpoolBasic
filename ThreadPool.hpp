namespace ThreadPool {
template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(
    F&& f, Args&&... args) {
  if (stop_all) {
    throw std::runtime_error("ThreadPool »ç¿ë ÁßÁöµÊ");
  }

  using return_type = typename std::result_of<F(Args...)>::type;
  auto job = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  std::future<return_type> job_result_future = job->get_future();
  {
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.push([job]() { (*job)(); });
  }

  cv_job_q_.notify_one();

  return job_result_future;
}

}  // namespace ThreadPool
 