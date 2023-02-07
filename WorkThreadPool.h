#pragma once

#include "BasicThreadPool.h"

#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include<vector>

static int g_wait_task_count = 0;
static std::mutex g_wait_task_mutex;

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

class WorkThreadPool : public ThreadNotifier, public BasicThreadPool{
public:
	WorkThreadPool(const int& num_threads, int queue_size = -1);
	~WorkThreadPool();

	// return false if queue is full or threadpool done
	bool insertTask(std::function<void()> f) override;

	int getAvailableCount() override;

	void InsertAvailableThread(const size_t& index);

private:
	void ThreadManaging();

	std::thread managing_thread;

	// Worker 쓰레드를 보관하는 벡터.
	std::vector<WorkThread*> worker_threads;

	std::deque<int> available_threads;
	std::mutex available_thread_mutex;

	std::mutex task_buffer_mutex;
};