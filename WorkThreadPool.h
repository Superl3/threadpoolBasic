#pragma once

#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <map>

class WorkThreadPool {
public:
	WorkThreadPool(const int& num_threads, int queue_size = -1);
	~WorkThreadPool();

	// return false if queue is full or threadpool done
	bool insertTask(std::function<void()> f);

	void restartWorkers();
	void forceRestartWorkers();
	int getThreadCount() { return worker_thread_count; }
	int getQueueSize() { return max_queue_size; }

	int getCurrentWorkingThread();
	int getQueuedTaskCount();

private:

	std::map<std::thread::id, bool> worker_thread_status;
	// �� Worker �������� ����.
	const int max_queue_size;
	const int worker_thread_count;

	// Worker �����带 �����ϴ� ����.
	std::deque<std::thread> worker_threads;
	// ���ϵ��� �����ϴ� job ť.
	std::deque<std::function<void()>> task_buffer;
	// ���� job ť�� ���� cv �� m.
	std::condition_variable task_buffer_cv;
	std::mutex task_buffer_mutex;

	std::mutex status_mutex;

	// ��� ������ ����
	bool stop_all;

	void createWorkers();
	void stopWorkers();

	// Worker ������
	void work();
};