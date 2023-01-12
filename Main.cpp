#include <cstdio>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "Buffer.h"
#include "ThreadPool.h"

#include "input.hpp"
#include "output.hpp"

int main(int argc, char* argv[]) {
  int threadCount = 100;
  if (argc >= 2) threadCount = atoi(argv[1]);

  int maxQueueSize = 100;
  if (argc >= 3) maxQueueSize = atoi(argv[2]);

  ThreadPool::ThreadPool pool(threadCount);

  std::mutex bufMutex;
  std::condition_variable mainCv;

  std::mutex outputMutex;
  std::condition_variable outputCv;

  std::thread inputThread(input, &pool, &bufMutex, &mainCv, &outputMutex, &outputCv);
  std::thread outputThread(output, &outputMutex, &outputCv);

  outputThread.detach();
  inputThread.join();
}
