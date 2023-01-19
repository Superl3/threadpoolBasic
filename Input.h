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
	Input(ThreadPoolManager *tpm_, Output *output);
	~Input();

	void execute();
private:
	enum class INPUTTYPE { CALC = 0, TEST, STOPTEST, QUIT, ERROR };

	void inputLoop();
	void doTest();
	INPUTTYPE parsedInput(const std::string& buf, calcData *data);
	void insertTask(calcData* data);

	bool stop_input = false;
	bool stop_test = false;

	std::thread input_thread;
	TestCreator* test_creator = nullptr;
	ThreadPoolManager* tpm = nullptr;

	Output* output = nullptr;

	std::vector<calcData*> input_database;
};