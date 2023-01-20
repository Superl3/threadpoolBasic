#include "ThreadPool.h"

ThreadPool::ThreadPool(const size_t& num_threads, size_t max_queue_size_)
    : thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
    CreateWorkers();
}
ThreadPool::~ThreadPool() {
    stop_all = true;
    taskBufferCV.notify_all();

    for (auto& t : workerThreads) {
        t.join();
    }
}

void ThreadPool::CreateWorkers() {
    for (size_t i = 0; i < thread_count; ++i) {
        workerThreads.emplace_back([this] { this->WorkerThread(); });
    }
}

void ThreadPool::WorkerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(taskBufferMutex);
            taskBufferCV.wait(lock, [this] { return !this->taskBuffer.empty() || stop_all; });
            if (stop_all && this->taskBuffer.empty()) break;
            task = std::move(taskBuffer.front());
            taskBuffer.pop_front();
        }
        task();
    }
}

bool ThreadPool::EnqueueJob(std::function<void()> f) {

    bool isInserted = false;

    if (stop_all) {
        throw std::runtime_error("ThreadPool 사용 중지됨");
    }
	else {
		if (taskBuffer.size() < max_queue_size) {
            {
                std::lock_guard<std::mutex> lock(taskBufferMutex);
                taskBuffer.emplace_back(f);
            }
            taskBufferCV.notify_one();
            isInserted = true;
        }
        else {
            throw std::runtime_error("ThreadPool 작업큐 가득 참");
        }
    }
    return isInserted;
}
