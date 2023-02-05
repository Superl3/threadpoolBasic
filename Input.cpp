#include <algorithm>
#include "Input.h"

#include<iostream>
Input::Input(ThreadPoolManager* tpm_, Output* output_, size_t test_case_count_)
	: tpm(tpm_), output(output_), test_creator(new TestCreator(test_case_count_))
{
}

void Input::execute() {
	input_thread = std::thread(&Input::inputLoop, this);
}

bool Input::insertTask(calcData* data, bool isTest) {
	bool isInserted = true;
	auto task = calcFactory(data, output, isTest);
	//std::cout << "2";
	isInserted = tpm->AddTask(task);
	//std::cout << "!0";
	if (!isTest) input_database.push_back(data);

	return isInserted;
}

Input::~Input() {
	stop_input = true;
	input_thread.join();

	delete test_creator;
	test_creator = nullptr;
}

#include <conio.h>

void Input::inputLoop() {

	printUsage();

	std::string inputBuffer = "";
	while (!stop_input) {
		char kbInput = _getch();
		if (kbInput == 13) {
			std::cout << '\n';
			calcData* data = new calcData;
			auto type = parsedInput(inputBuffer, data);
			switch (type) {
			case INPUTTYPE::CALC: {
				insertTask(data);
				break;
			}
			case INPUTTYPE::TEST: {
				stop_test = false;
				std::thread testThread(&Input::doTest, this);
				testThread.detach();
				break;
			}
			case INPUTTYPE::STOPTEST: {
				stop_test = true;
				tpm->ForceQuitTest();
				break;
			}
			case INPUTTYPE::INFO: {
				printTPMInfo();
				break;
			}
			case INPUTTYPE::QUIT: {
				stop_input = true;
				break;
			}
			default:
				printUsage();
				break;
			}
			inputBuffer = "";
			continue;
		}
		if (kbInput == 8) {
			if (inputBuffer.size() > 0) {
				std::cout << "\b \b";
				inputBuffer.erase(inputBuffer.end() - 1);
			}
		}
		else {
			std::cout << kbInput;
			inputBuffer += kbInput;
		}
	}
}

void Input::doTest() {
	test_time_checker = new GlobalPerformanceMonitor();
	auto testData = test_creator->getCreatedTest();
	//std::cout << "!4";
	test_time_checker->testStart();
	//std::cout << "!3";
	if (!tpm->isDisabled()) {
		std::cout << "start do test.\n";
		for (auto data : testData) {
			//std::cout << "1";

			if (!insertTask(data, true)) {
				std::cout << "ERROR Occurred while testing.\n";
				stop_test = true;
			}
			if (stop_test) break;
		}
		std::cout << "start insert task done.\n";
	}
	else {
		for (auto data : testData) {
			auto calc = calcFactory(data, output, true);
			calc->execute();
			if (stop_test) break;
		}
	}
	//std::cout << "!1";
	tpm->StopForTestEnd(testData.size());
	//std::cout << "!2";
	if (!stop_test) {
		test_time_checker->setEndTimer();
		std::cout << "THREAD COUNT : " << tpm->getWorkThreadCount() << " TESTCASE COUNT : " << testData.size() << "\n" <<
			"TOTAL RUNNING TIME : " << test_time_checker->getRunningTime() << "ms\n";
	}
	//std::cout << "!3";
	delete test_time_checker; 
	stop_test = true;
}

void Input::printUsage() {
	std::string usage_string =
		"\nthreadpoolBasic (작업스레드 수) (작업큐 사이즈) (로그 파일명) (테스트데이터 수)\n\n숫자 기호 숫자\"를 입력하시면 연산이 실행됩니다. (예: 1 + 3)\ntest : 사전에 생성해 둔 데이터를 바탕으로 테스트를 실행합니다.\nstoptest : test가 진행되는 도중 중단합니다\ninfo : 스레드풀의 정보를 출력합니다.\nquit : 프로그램을 종료합니다.\n\n콘솔 및 파일 출력 양식은 다음과 같습니다:\n입력\t\t\t결과\t소요시간(ms)\n\n";
	std::cout << usage_string;
}
void Input::printTPMInfo() {
	std::cout << tpm->getTPMinfo();
}

Input::INPUTTYPE Input::parsedInput(const std::string& buf, calcData* data) {
	auto splitData = [&buf]() -> std::vector<std::string> {
		size_t cur = 0, prev = 0;
		std::vector<std::string> splitedBuf;
		do {
			cur = buf.find(' ', prev);
			std::string item;

			if (cur == -1) item = buf.substr(prev);
			else item = buf.substr(prev, cur - prev);

			if (item.length() > 0)
				splitedBuf.push_back(item);

			prev = cur + 1;
		} while (prev);
		return splitedBuf;
	};
	auto getOperatorFromChar = [](char& input) {
		OPERATOR op = OPERATOR::NONE;
		switch (input) {
		case '+':
			op = OPERATOR::PLUS;
			break;
		case '-':
			op = OPERATOR::MINUS;
			break;
		case '*':
			op = OPERATOR::MULTIPLY;
			break;
		case '/':
			op = OPERATOR::DIVIDE;
			break;
		}
		return op;
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

			data->first = atoi(splitedBuf[0].c_str());
			data->op = getOperatorFromChar(splitedBuf[1][0]);
			data->second = atoi(splitedBuf[2].c_str());
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
		else if (toLower(splitedBuf[0]) == "info")
			ret = INPUTTYPE::INFO;
		else if (toLower(splitedBuf[0]) == "exit" || splitedBuf[0] == "quit")
			ret = INPUTTYPE::QUIT;
	}
	if (ret != INPUTTYPE::ERROR);
	return ret;
}