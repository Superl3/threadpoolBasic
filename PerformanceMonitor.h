#pragma once

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

		elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
		return true;
	}
private:

	bool start_time_checked = false, end_time_checked = false;
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;
};
