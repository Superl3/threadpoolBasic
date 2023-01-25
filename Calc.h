#pragma once

#include "Task.h"
#include "Output.h"

enum class OPERATOR { PLUS, MINUS, MULTIPLY, DIVIDE, NONE };

struct calcData {
	int first = 0, second = 0;
	OPERATOR op = OPERATOR::NONE;
};

class Output;

class Calc : public Task<int, int> {
protected:
	std::function<int(int, int)> calc_func = NULL;
	int result = 0;

	calcData* input_ref;
	Output* output_handler;

	void init() {
	};

public:
	Calc(calcData* data, Output* out) : input_ref(data), output_handler(out), Task() {
		init();
	}
	decltype(calc_func) get() {
		return calc_func;
	}

	void execute() {
		result = calc_func(input_ref->first, input_ref->second);
		performance_monitor.setEndTimer();
	}

	void callback() {
		int elapsed_time = 0;
		bool bSuccess = performance_monitor.getRunningTime(elapsed_time);
		if (bSuccess) output_handler->process(input_ref, result, elapsed_time);
	}

	char displayOperator() {
		return '=';
	}
};

class Plus : public Calc {
protected:
	void init() {
		calc_func = [](const int& a, const int& b) { return a + b; };
	}

public:
	Plus(calcData* data, Output* out) : Calc(data, out) {
		init();
	}

	char displayOperator() {
		return '+';
	}
};

class Minus : public Calc {
protected:
	void init(){
		calc_func = [](const int& a, const int& b) { return a - b; };
	}

public:
	Minus(calcData* data, Output* out) : Calc(data, out) {
		init();
	}
	char displayOperator() {
		return '-';
	}
};

class Multiply : public Calc {
protected:
	void init() {
		calc_func = [](const int& a, const int& b) { return a * b; };
	}

public:
	Multiply(calcData* data, Output* out) : Calc(data, out) {
		init();
	}
	char displayOperator() {
		return '*';
	}
};

class Divide : public Calc {
protected:
	void init() {
		calc_func = [](const int& a, const int& b) { return a / b; };
	}

public:
	Divide(calcData* data, Output* out) : Calc(data, out) {
		init();
	}
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

static Calc* calcFactory(calcData* data, Output* out) {

	Calc* created_calc = nullptr;

	switch (data->op) {
	case OPERATOR::PLUS:
		created_calc = new Plus(data, out);
		break;
	case OPERATOR::MINUS:
		created_calc = new Minus(data, out);
		break;
	case OPERATOR::MULTIPLY:
		created_calc = new Multiply(data, out);
		break;
	case OPERATOR::DIVIDE:
		created_calc = new Divide(data, out);
		break;
	}
	created_calc->setArgs(data->first, data->second);

	return created_calc;
}