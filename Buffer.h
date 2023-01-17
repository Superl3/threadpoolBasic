#pragma once

#include <mutex>
#include <thread>
#include <string>
#include <list>
#include <future>

class Operator;

class Operator {
public:
	enum class OPERATOR { PLUS, MINUS, MULTIPLY, DIVIDE, NONE };
	Operator(OPERATOR _op) : op(_op) {}
	Operator(char raw) : op(fromChar(raw)) {}
	Operator(const int& random_number) : op(fromRandom(random_number)) {}
	static Operator::OPERATOR fromChar(const char& raw) {
		switch (raw) {
		case '+':
			return OPERATOR::PLUS;
		case '-':
			return OPERATOR::MINUS;
		case '/':
			return OPERATOR::DIVIDE;
		case '*':
			return OPERATOR::MULTIPLY;
		default:
			return OPERATOR::NONE;
		}
	}
	static OPERATOR fromRandom(const int& rand) {
		switch (rand) {
		case 0:
			return OPERATOR::PLUS;
		case 1:
			return OPERATOR::MINUS;
		case 2:
			return OPERATOR::DIVIDE;
		case 3:
			return OPERATOR::MULTIPLY;
		default:
			return OPERATOR::NONE;
		}
	}
	int calc(const int &a, const int &b) {
		switch (op) {
		case OPERATOR::PLUS:
			return a + b;
		case OPERATOR::MINUS:
			return a - b;
		case OPERATOR::DIVIDE:
			return a / b;
		case OPERATOR::MULTIPLY:
			return a * b;
		default:
			return 0;
		}
	}
	char toChar() {
		switch (op) {
		case OPERATOR::PLUS:
			return '+';
		case OPERATOR::MINUS:
			return '-';
		case OPERATOR::DIVIDE:
			return '/';
		case OPERATOR::MULTIPLY:
			return '*';
		default:
			return ' ';
		}
	}

private:
	OPERATOR op = OPERATOR::NONE;
};

struct calcData {
	int first = 0, second = 0;
	Operator op = Operator::OPERATOR::NONE;
};

static int calc(calcData& data) {
	return data.op.calc(data.first, data.second);
}