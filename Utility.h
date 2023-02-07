#pragma once
#define timeStandard std::chrono::milliseconds
#define timeTaskSleep std::this_thread::sleep_for(std::chrono::milliseconds(1))

#define TEST_1000000 "test.txt"

#define TEST_COUNT 1000000
#define THREAD_COUNT 8

#define THREAD_POLLING 1
#define DISPLAY_PROCESSED_TEST 1

#define TEST_FILE TEST_1000000

#define OLD_POOL ThreadPool
#define NEW_POOL WorkThreadPool

#define POOLMETHOD OLD_POOL