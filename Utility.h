#pragma once
#define timeStandard std::chrono::milliseconds
#define timeTaskSleep std::this_thread::sleep_for(std::chrono::milliseconds(1))

#define TEST_COUNT 10000
#define THREAD_COUNT 8

#define THREAD_POOLING 1
//#define DISPLAY_PROCESSED_TEST 1