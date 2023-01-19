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

  // job �� �߰��Ѵ�.
  //template <class F>
  //void EnqueueJob(F&& f, std::function<void(std::result_of_t<F>)> callback = nullptr);

  void EnqueueJob(std::function<void()> f);

 private:
  // �� Worker �������� ����.
  const size_t workerThreadCount;
  // Worker �����带 �����ϴ� ����.
  std::deque<std::thread> workerThreads;
  // ���ϵ��� �����ϴ� job ť.
  std::deque<std::function<void()>> taskBuffer;
  // ���� job ť�� ���� cv �� m.
  std::condition_variable taskBufferCV;
  std::mutex taskBufferMutex;

  // ��� ������ ����
  bool stop_all;

  void CreateWorkers();

  // Worker ������
  void WorkerThread();
};

#include "ThreadPool.hpp"