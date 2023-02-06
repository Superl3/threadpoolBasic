#pragma once
#define timeStandard std::chrono::milliseconds
#define timeTaskSleep std::this_thread::sleep_for(std::chrono::milliseconds(1))

#define TEST_COUNT 1000000
#define THREAD_COUNT 16
