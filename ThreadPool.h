#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Task.h"

class ThreadPool {
 public:
  ThreadPool(size_t num_threads);
  ~ThreadPool();

  // job 을 추가한다.
  //template <class F>
  //void EnqueueJob(F&& f, std::function<void(std::result_of_t<F>)> callback = nullptr);

  void EnqueueJob(std::function<void()> f);

 private:
  // 총 Worker 쓰레드의 개수.
  const size_t workerThreadCount;
  // Worker 쓰레드를 보관하는 벡터.
  std::deque<std::thread> workerThreads;
  // 할일들을 보관하는 job 큐.
  std::deque<std::function<void()>> taskBuffer;
  // 위의 job 큐를 위한 cv 와 m.
  std::condition_variable taskBufferCV;
  std::mutex taskBufferMutex;

  // 모든 쓰레드 종료
  bool stop_all;

  void CreateWorkers();

  // Worker 쓰레드
  void WorkerThread();
};

#include "ThreadPool.hpp"