#include "Input.h"

Input::Input() {
	test_creator = new TestCreator(100);
}

void Input::execute() {
	input_thread = std::thread(&Input::inputLoop, this);
}

Input::~Input() {
	stop_input = true;
	input_thread.join();

	delete test_creator;
	test_creator = nullptr;
}

void Input::doTest() {
	while (!stop_test) {

		calcData data;
		if (!test_creator->getSingleTest(data)) {
			stop_test = true;
			printf("Test Done\n");
			break;
		}

		// pool->EnqueueJob(calcData::calc, data);
		 // std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

Input::INPUTTYPE Input::parsedInput(const std::string& buf, calcData& parsedData) {
	auto splitData = [&buf]() -> std::vector<std::string> {
		size_t cur = 0, prev = 0;
		std::vector<std::string> splitedBuf;
		while (true) {
			cur = buf.find(' ', prev);
			if (cur == -1) {
				splitedBuf.push_back(buf.substr(prev));
				break;

			}
			splitedBuf.push_back(buf.substr(prev, cur - prev));
			prev = cur + 1;
		}
		return splitedBuf;
	};
	std::vector<std::string> splitedBuf = splitData();

	INPUTTYPE ret = INPUTTYPE::ERROR;

	if (splitedBuf.size() == 3) {
		auto isLegit = [](std::string a, std::string search) -> bool {
			return a.find_first_not_of(search) == std::string::npos;
		};
		std::string NUMBER = "0.123456789", OPERATORS = "+-*/";

		if (isLegit(splitedBuf[0], NUMBER) && isLegit(splitedBuf[1], OPERATORS) &&
			isLegit(splitedBuf[2], NUMBER) && splitedBuf[1].length() == 1) {

			parsedData.first = atoi(splitedBuf[0].c_str());
			parsedData.op = Operator(splitedBuf[1][0]);
			parsedData.second = atoi(splitedBuf[2].c_str());
			ret = INPUTTYPE::CALC;
		}
	}
	else if (splitedBuf.size() == 1) {
		auto toLower = [](std::string s) -> std::string {
			std::transform(s.begin(), s.end(), s.begin(), tolower);
			return s;
		};

		if (toLower(splitedBuf[0]) == "test")
			ret = INPUTTYPE::TEST;
		else if (toLower(splitedBuf[0]) == "stoptest")
			ret = INPUTTYPE::STOPTEST;
		else if (toLower(splitedBuf[0]) == "exit" || splitedBuf[0] == "quit")
			ret = INPUTTYPE::QUIT;
	}
	if (ret != INPUTTYPE::ERROR) printf("%s\n", buf.c_str());
	return ret;
}

#include <conio.h>

void Input::inputLoop() {
	std::string inputBuffer = "";
	while (!stop_input) {
		char kbInput = _getch();
		if (kbInput == 13) {
			calcData data;
			auto type = parsedInput(inputBuffer, data);
			switch (type) {
			case INPUTTYPE::CALC: {
				//pool->EnqueueJob(calcData::calc, data);
				break;
			}
			case INPUTTYPE::TEST: {
				if (!stop_test) {
					stop_test = false;

					std::thread testThread(&Input::doTest, this);
					testThread.detach();

				}
				break;
			}
			case INPUTTYPE::STOPTEST: {
				stop_test = true;
				break;
			}
			case INPUTTYPE::QUIT: {
				return;
			}
			}
			inputBuffer = "";
			continue;
		}
		inputBuffer += kbInput;
	}
}