#include "ThreadPool.h"

ThreadPool::ThreadPool(const size_t& num_threads, size_t max_queue_size_)
    : worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
    createWorkers();
}
ThreadPool::~ThreadPool() {
    stopWorkers();
}

void ThreadPool::stopWorkers() {
    stop_all = true;
    task_buffer_cv.notify_all();

    for (auto& t : worker_threads) {
        t.join();
    }
    worker_threads.clear();
}

void ThreadPool::restartWorkers() {
    stopWorkers();
    stop_all = false;
    createWorkers();
}


void ThreadPool::createWorkers() {
    for (size_t i = 0; i < worker_thread_count; ++i) {
        worker_threads.emplace_back([this] { this->work(); });
    }
}

#include<iostream>

void ThreadPool::work() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(task_buffer_mutex);
            task_buffer_cv.wait(lock, [this] { return !this->task_buffer.empty() || stop_all; });
            if (stop_all && this->task_buffer.empty()) break;
            task = std::move(task_buffer.front());
            task_buffer.pop_front();
        }
        task();
    }
}

bool ThreadPool::insertTask(std::function<void()> f) {

    bool isInserted = false;

    if (stop_all) {
        throw std::runtime_error("ThreadPool 사용 중지됨");
    }
	else {
		if (task_buffer.size() < max_queue_size) {
            {
                std::lock_guard<std::mutex> lock(task_buffer_mutex);
                task_buffer.emplace_back(f);
            }
            task_buffer_cv.notify_one();
            isInserted = true;
        }
        else {
            throw std::runtime_error("ThreadPool 작업큐 가득 참");
        }
    }
    return isInserted;
}
