#include "Input.h"

Input::Input(ThreadPoolManager *tpm_, Output *output_, size_t test_case_count_)
	: tpm(tpm_), output(output_), test_creator(new TestCreator(test_case_count_))
{
}

void Input::execute() {
	input_thread = std::thread(&Input::inputLoop, this);
}

void Input::insertTask(calcData* data) {
	auto task = calcFactory(data, output);
	tpm->AddTask(task);
}

Input::~Input() {
	stop_input = true;
	input_thread.join();

	delete test_creator;
	test_creator = nullptr;
}

#include <conio.h>
#include<iostream>

void Input::inputLoop() {
	std::string inputBuffer = "";
	while (!stop_input) {
		char kbInput = _getch();
		if (kbInput == 13) {
			calcData* data = new calcData;
			auto type = parsedInput(inputBuffer, data);
			switch (type) {
			case INPUTTYPE::CALC: {
				insertTask(data);
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
				stop_input = true;
				break;
			}
			default:
				printUsage();
				break;
			}
			inputBuffer = "";
			std::cout << '\n';
			continue;
		}
		if (kbInput == 8) {
			std::cout << "\b \b";
			inputBuffer.erase(inputBuffer.end() - 1);
		}
		else {
			std::cout << kbInput;
			inputBuffer += kbInput;
		}
	}
}

void Input::doTest() {
	while (!stop_test) {

		calcData* data = new calcData;
		if (!test_creator->getSingleTest(data)) {
			stop_test = true;
			printf("Test Done\n");
			break;
		}

		insertTask(data);
	}
}

void Input::printUsage() {
	std::string usage_string =
		"threadpoolBasic (작업스레드 수) (작업큐 사이즈) (로그 파일명) (테스트데이터 수)\n\n숫자' '기호' '숫자\"를 입력하시면 연산이 실행됩니다.\ntest : 사전에 생성해 둔 데이터를 바탕으로 테스트를 실행합니다.\nstoptest : test가 진행되는 도중 중단합니다\nquit : 프로그램을 종료합니다.";
	std::cout << usage_string;
}

Input::INPUTTYPE Input::parsedInput(const std::string& buf, calcData *data) {
	auto splitData = [&buf]() -> std::vector<std::string> {
		size_t cur = 0, prev = 0;
		std::vector<std::string> splitedBuf;
		do {
			cur = buf.find(' ', prev);
			std::string item;

			if (cur == -1) item = buf.substr(prev);
			else item = buf.substr(prev, cur - prev);

			if(item.length() > 0)
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
		else if (toLower(splitedBuf[0]) == "exit" || splitedBuf[0] == "quit")
			ret = INPUTTYPE::QUIT;
	}
	if (ret != INPUTTYPE::ERROR);
	return ret;
}