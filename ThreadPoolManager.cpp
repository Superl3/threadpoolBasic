#include"ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager(const size_t& num_threads, size_t output_threads)
	: taskPool(num_threads), resultPool(output_threads){

}
