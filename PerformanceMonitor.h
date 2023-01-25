#pragma once
#include "Utility.h"

#include <chrono>

class PerformanceMonitor {
public:
	PerformanceMonitor() {};

	void setStartTimer() {
		if (!start_time_checked) {
			start_time = std::chrono::steady_clock::now();
			start_time_checked = true;
		}
	}
	void setEndTimer() {
		if (!end_time_checked) {
			end_time = std::chrono::steady_clock::now();
			end_time_checked = true;
		}
	}
	bool getRunningTime(int &elapsed_time) {
		if (!start_time_checked || !end_time_checked)
			return false;

		elapsed_time = std::chrono::duration_cast<timeStandard>(end_time - start_time).count();
		return true;
	}
private:

	bool start_time_checked = false, end_time_checked = false;
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;
};


#include<mutex>
static class OverallPerformanceMonitor : public PerformanceMonitor {
private:
	int count = 0;
	std::chrono::steady_clock::duration sumOfRunningTime = std::chrono::steady_clock::duration::zero();

	std::mutex performance_monitor_mutex;

public:
	template<typename T>
	void addTask(int duration) {
		std::lock_guard<std::mutex> lock(performance_monitor_mutex);
		count += 1;
		sumOfRunningTime += T(duration);
	}

	void clearTask() {
		std::lock_guard<std::mutex> lock(performance_monitor_mutex);
		count = 0;
		sumOfRunningTime = std::chrono::steady_clock::duration::zero();
	}

	template<typename T>
	int getAverageRunningTime() {
		return std::chrono::duration_cast<T>(sumOfRunningTime / count).count();
	}
};