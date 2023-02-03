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

	// 총 Worker 쓰레드의 개수.
	const int max_queue_size;
	const int worker_thread_count;

	std::thread managing_thread;

	// Worker 쓰레드를 보관하는 벡터.
	std::vector<WorkThread*> worker_threads;

	std::deque<WorkThread*> available_threads;
	std::mutex available_thread_mutex;

	// 할일들을 보관하는 job 큐.
	std::deque<std::function<void()>> task_buffer;
	// 위의 job 큐를 위한 cv 와 m.
	std::condition_variable task_buffer_cv;
	std::mutex task_buffer_mutex;


	// 모든 쓰레드 종료
	bool stop_all;

	// Worker 쓰레드
};