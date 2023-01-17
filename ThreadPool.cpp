#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads) 
    : workerThreadCount(num_threads), stop_all(false) {
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
    for (size_t i = 0; i < workerThreadCount; ++i) {
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

void ThreadPool::EnqueueJob(std::function<void()> f) {

    if (stop_all) {
        throw std::runtime_error("ThreadPool ��� ������");
    }
    {
        std::lock_guard<std::mutex> lock(taskBufferMutex);
        taskBuffer.emplace_back(f);
    }
    taskBufferCV.notify_one();
    return;
}
