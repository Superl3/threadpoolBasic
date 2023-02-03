#include "WorkThreadPool.h"

WorkThreadPool::WorkThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
	if (worker_thread_count > 0) {
		for (size_t i = 0; i < worker_thread_count; ++i) {
			auto thread = new WorkThread(i, this);
			worker_threads.emplace_back(thread);
			available_threads.push_back(thread);
		}
	}

	managing_thread = std::thread(&WorkThreadPool::ThreadManaging, this);
}
WorkThreadPool::~WorkThreadPool() {
	stop_all = true;

	managing_thread.join();

	/*for (auto& t : worker_threads) {
		t.join();
	}*/
	worker_threads.clear();
}

void WorkThreadPool::ThreadManaging() {
	while (true) {
		WorkThread* thread = nullptr; // 이렇게 짜면 문제점 : 종료시 작업이 남아있는지 확인이 불가능하다 : 특수처리 해주어야함 TODO
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			std::lock_guard<std::mutex> lock_for_available_thread(available_thread_mutex);
			if (available_threads.empty())
				continue;

			thread = std::move(available_threads.front());
			available_threads.pop_front();
			break;
		}

		std::function<void()> task;
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			std::lock_guard<std::mutex> lock_for_buffer(task_buffer_mutex);
			if (task_buffer.empty()) {
				if (stop_all) break;
				continue;
			}
			task = std::move(task_buffer.front());
			task_buffer.pop_front();
			// pooling task queue
			break;
		}
		thread->assignTask(task);
		if (stop_all) break;
	}
}


WorkThread::WorkThread(int index, ThreadNotifier* noti) : id(index), notifier(noti) {
	thread = std::thread(&WorkThread::work, this);
}

void WorkThread::work() {
	while (true) {
		//worker_thread_status[thread_id] = true;
		{
			std::unique_lock<std::mutex> lock(thread_mutex);
			thread_cv.wait(lock, [this] { return task; });
			task();
			task = NULL;
			notifier->InsertAvailableThread(id);
		}
		//worker_thread_status[thread_id] = true;
	}
}
void WorkThread::assignTask(std::function<void()> task_) {
	task = task_;
	thread_cv.notify_all();
}
int WorkThreadPool::getQueuedTaskCount() {
	return task_buffer.size();
}

void WorkThreadPool::InsertAvailableThread(const size_t &index) {
	std::lock_guard<std::mutex> lock(available_thread_mutex);
	available_threads.push_back(worker_threads[index]);
}

bool WorkThreadPool::insertTask(std::function<void()> f) {

	bool isInserted = false;

	if (stop_all) {
	}
	else {
		if (task_buffer.size() < max_queue_size || max_queue_size == -1) {
			{
				std::lock_guard<std::mutex> lock(task_buffer_mutex);
				task_buffer.emplace_back(f);
			}
			isInserted = true;
		}
	}
	return isInserted;
}
