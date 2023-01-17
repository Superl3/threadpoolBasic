#pragma once

#include <thread>
#include <string>

#include "Buffer.h"
#include "TestCreator.h"

class Input {
public:
	Input();
	~Input();

	void execute();
private:
	enum class INPUTTYPE { CALC = 0, TEST, STOPTEST, QUIT, ERROR };

	void inputLoop();
	void doTest();
	INPUTTYPE parsedInput(const std::string& buf, calcData& parsedData);

	bool stop_input = false;
	bool stop_test = false;

	std::thread input_thread;
	TestCreator* test_creator;
};