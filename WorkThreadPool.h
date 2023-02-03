#pragma once

#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include<vector>

class ThreadNotifier {
public:
	virtual void InsertAvailableThread(const size_t& index) = 0;
};

class WorkThread {
	std::condition_variable thread_cv;
	std::mutex thread_mutex;

	std::thread thread;
	const size_t id;

	bool stop = false;

	ThreadNotifier* notifier = NULL;

	std::function<void()> task = NULL;
	void work();

public:
	WorkThread(int index, ThreadNotifier *noti);
	~WorkThread();

	size_t getID() { return id; }
	void assignTask(std::function<void()> task_);
};

class WorkThreadPool : public ThreadNotifier{
public:
	WorkThreadPool(const int& num_threads, int queue_size = -1);
	~WorkThreadPool();

	void InsertAvailableThread(const size_t &index);

	// return false if queue is full or threadpool done
	bool insertTask(std::function<void()> f);

	int getThreadCount() { return worker_thread_count; }
	int getQueueSize() { return max_queue_size; }

	int getQueuedTaskCount();

private:
	void ThreadManaging();

	// �� Worker �������� ����.
	const int max_queue_size;
	const int worker_thread_count;

	std::thread managing_thread;

	// Worker �����带 �����ϴ� ����.
	std::vector<WorkThread*> worker_threads;

	std::deque<WorkThread*> available_threads;
	std::mutex available_thread_mutex;

	// ���ϵ��� �����ϴ� job ť.
	std::deque<std::function<void()>> task_buffer;
	// ���� job ť�� ���� cv �� m.
	std::condition_variable task_buffer_cv;
	std::mutex task_buffer_mutex;


	// ��� ������ ����
	bool stop_all;

	// Worker ������
};