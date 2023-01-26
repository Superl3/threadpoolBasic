#pragma once

#include <deque>
#include <thread>
#include <mutex>
#include <functional>

class ThreadPool {
 public:
  ThreadPool(const size_t &num_threads, size_t queue_size = 100000);
  ~ThreadPool();

  // return false if queue is full or threadpool done
  bool insertTask(std::function<void()> f);

  bool isEmpty() {
	  return task_buffer.empty();
  }

  void restartWorkers();

 private:
  // �� Worker �������� ����.
  const size_t max_queue_size;
  const size_t worker_thread_count;

  // Worker �����带 �����ϴ� ����.
  std::deque<std::thread> worker_threads;
  // ���ϵ��� �����ϴ� job ť.
  std::deque<std::function<void()>> task_buffer;
  // ���� job ť�� ���� cv �� m.
  std::condition_variable task_buffer_cv;
  std::mutex task_buffer_mutex;

  // ��� ������ ����
  bool stop_all;

  void createWorkers();
  void stopWorkers();

  // Worker ������
  void work();
};