#pragma once

#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <map>

class ThreadPool {
public:
	ThreadPool(const int& num_threads, int queue_size = -1);
	~ThreadPool();

	// return false if queue is full or threadpool done
	bool insertTask(std::function<void()> f);

	int getThreadCount() { return worker_thread_count; }
	int getQueueSize() { return max_queue_size; }

	int getAvailableCount();
	int getQueuedTaskCount();

private:

	// 총 Worker 쓰레드의 개수.
	const int max_queue_size;
	const int worker_thread_count;

	// Worker 쓰레드를 보관하는 벡터.
	std::deque<std::thread> worker_threads;
	// 할일들을 보관하는 job 큐.
	std::deque<std::function<void()>> task_buffer;
	// 위의 job 큐를 위한 cv 와 m.
	std::condition_variable task_buffer_cv;
	std::mutex task_buffer_mutex;

	std::mutex status_mutex;

	// 모든 쓰레드 종료
	bool stop_all;

	// Worker 쓰레드
	void work();
};