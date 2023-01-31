#pragma once

#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <map>

class ThreadPool {
public:
	ThreadPool(const size_t& num_threads, size_t queue_size = -1);
	~ThreadPool();

	// return false if queue is full or threadpool done
	bool insertTask(std::function<void()> f);

	void restartWorkers();
	void forceRestartWorkers();
	size_t getThreadCount() { return worker_thread_count; }
	size_t getQueueSize() { return max_queue_size; }

	int getCurrentWorkingThread();
	int getQueuedTaskCount();

private:

	std::map<std::thread::id, bool> worker_thread_status;
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