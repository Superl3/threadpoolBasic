#include "BasicThreadPool.h"

BasicThreadPool::BasicThreadPool(const int& num_threads, int max_queue_size_)
	: worker_thread_count(num_threads), max_queue_size(max_queue_size_), stop_all(false) {
}
BasicThreadPool::~BasicThreadPool() {
}