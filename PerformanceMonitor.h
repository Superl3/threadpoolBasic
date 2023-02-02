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
	int getRunningTime() {
		int duration = -1;

		if (start_time_checked && end_time_checked)
			duration = std::chrono::duration_cast<timeStandard>(end_time - start_time).count();

		return duration;
	}
private:

	bool start_time_checked = false, end_time_checked = false;
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;
};

class GlobalPerformanceMonitor : public PerformanceMonitor {
private:
	bool isStarted;
public:
	bool checkStarted() {
		return isStarted;
	}
	void testStart() {
		isStarted = true;
		setStartTimer();
	}
};