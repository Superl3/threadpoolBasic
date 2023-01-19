#pragma once
#include<functional>
#include<utility>
#include<tuple>

#include "PerformanceMonitor.h"

template <typename... Args>
class Task {
	enum class TYPE {
		CALC, ADD, MINUS, MULTIPLY, DIVIDE
	};

public:
	Task()
	{
		init();
	}
	Task(void* f, Args... args) : func_(f), args_(args ...)
	{
		init();
	}

protected:
	TYPE task_type;

	void *func_;
	std::tuple<Args...> args_;

	PerformanceMonitor performance_monitor;

	void init() {
		performance_monitor.setStartTimer();
	}

public:
	void setArgs(Args... args) {
		args_ = std::make_tuple(args...);
	}

	void setArgs(std::tuple<Args...> args) {
		args_ = std::forward(args);
	}

	std::tuple<Args...> getArgs() {
		return args_;
	}

	virtual void execute() {
		performance_monitor.setEndTimer();
	}

	virtual void callback() {
	}
};