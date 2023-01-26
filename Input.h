#pragma once

#include <thread>
#include <string>
#include <vector>

#include "TestCreator.h"
#include "ThreadPoolManager.h"
#include "Calc.h"

#include "Output.h"

class Input {
public:
	Input(GlobalPerformanceMonitor* test_time_checker_, ThreadPoolManager *tpm_, Output *output, size_t test_case_count_);
	~Input();

	void execute();
	bool isDone() {
		return stop_input;
	}
private:
	enum class INPUTTYPE { CALC = 0, TEST, STOPTEST, QUIT, ERROR };
	INPUTTYPE parsedInput(const std::string& buf, calcData* data);

	void doTest();
	void inputLoop();
	void insertTask(calcData* data, bool isTest = false);

	void printUsage();

	bool stop_input = false;
	bool stop_test = false;

	std::thread input_thread;
	TestCreator* test_creator = nullptr;
	GlobalPerformanceMonitor* test_time_checker = nullptr;


	ThreadPoolManager* tpm = nullptr;

	Output* output = nullptr;

	std::deque<calcData*> input_database;
};