#pragma once

#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <map>

class BasicThreadPool {
public:
	BasicThreadPool(const int& num_threads, int queue_size = -1);
	~BasicThreadPool();

	// return false if queue is full or BasicThreadPool done
	virtual bool insertTask(std::function<void()> f) = 0;

	int getThreadCount() { return worker_thread_count; }
	int getQueueSize() { return max_queue_size; }

	virtual int getAvailableCount() = 0;
	int getQueuedTaskCount() {
		return task_buffer.size();
	}

protected:

	// 총 Worker 쓰레드의 개수.
	const int max_queue_size;
	const int worker_thread_count;

	// 할일들을 보관하는 job 큐.
	std::deque<std::function<void()>> task_buffer;

	// 모든 쓰레드 종료
	bool stop_all;
};