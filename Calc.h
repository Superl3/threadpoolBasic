#pragma once

#include "Output.h"

#include<thread>

enum class OPERATOR { PLUS, MINUS, MULTIPLY, DIVIDE, NONE };

struct calcData {
	int first = 0, second = 0;
	OPERATOR op = OPERATOR::NONE;
};

class Output;

class Calc {
private : 
	PerformanceMonitor performance_monitor;

protected:
	virtual int calculate(const int& a, const int& b) = 0;
	int result = 0;

	calcData* input_ref;
	Output* output_handler;

	const bool is_test;

public:
	Calc(calcData* data, Output* out, bool isTest_ = false) : input_ref(data), output_handler(out), is_test(isTest_) {
	}

	int execute() {
		result = calculate(input_ref->first, input_ref->second);
		performance_monitor.setEndTimer();
		return result;
	}

	void callback(const int &result) {
		output_handler->process(input_ref, result, performance_monitor.getRunningTime(), is_test);
	}

	char displayOperator() {
		return '=';
	}
};

class Plus : public Calc {
private:
	static int plusFunc(const int& a, const int& b) { return a + b; };
protected:
	int calculate(const int& a, const int& b) {
		return a + b;
	}

public:
	Plus(calcData* data, Output* out, bool is_test_ = false) : Calc(data, out, is_test_) {}
	char displayOperator() {
		return '+';
	}
};

class Minus : public Calc {
private:
	static int minusFunc(const int& a, const int& b) { return a - b; };
protected:
	int calculate(const int& a, const int& b) {
		return a - b;
	}

public:
	Minus(calcData* data, Output* out, bool is_test_ = false) : Calc(data, out, is_test_) {}
	char displayOperator() {
		return '-';
	}
};

class Multiply : public Calc {
private:
	static int multiplyFunc(const int& a, const int& b) { return a * b; };
protected:
	int calculate(const int& a, const int& b) {
		return a * b;
	}

public:
	Multiply(calcData* data, Output* out, bool is_test_ = false) : Calc(data, out, is_test_) {}
	char displayOperator() {
		return '*';
	}
};

class Divide : public Calc {
private:
	static int divideFunc(const int& a, const int& b) { return a / b; };
protected:
	int calculate(const int& a, const int& b) {
		return a / b;
	}

public:
	Divide(calcData* data, Output* out, bool is_test_ = false) : Calc(data, out, is_test_) {}
	char displayOperator() {
		return '/';
	}
};

static char operatorToChar(OPERATOR op) {
	char ch = ' ';
	switch (op) {
	case OPERATOR::PLUS:
		ch = '+';
		break;
	case OPERATOR::MINUS:
		ch = '-';
		break;
	case OPERATOR::MULTIPLY:
		ch = '*';
		break;
	case OPERATOR::DIVIDE:
		ch = '/';
		break;
	}
	return ch;
}

static Calc* calcFactory(calcData* data, Output* out, bool is_test = false) {

	Calc* created_calc = nullptr;

	switch (data->op) {
	case OPERATOR::PLUS:
		created_calc = new Plus(data, out, is_test);
		break;
	case OPERATOR::MINUS:
		created_calc = new Minus(data, out, is_test);
		break;
	case OPERATOR::MULTIPLY:
		created_calc = new Multiply(data, out, is_test);
		break;
	case OPERATOR::DIVIDE:
		created_calc = new Divide(data, out, is_test);
		break;
	}
	return created_calc;
}