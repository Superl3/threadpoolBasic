#pragma once
#define timeStandard std::chrono::milliseconds
#define timeTaskSleep std::this_thread::sleep_for(std::chrono::microseconds(1))
