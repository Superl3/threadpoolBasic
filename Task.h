#pragma once
#include<utility>
#include<tuple>
#include<functional>

class AbstractTask {
public :
	virtual ~AbstractTask() {};
	virtual void execute() = 0;
};

template <typename Func, typename... Args>
class Task : public AbstractTask {
public :
	using ReturnType = std::invoke_result_t<Func(Args...)>;;
	using Callback = std::function<void(ReturnType)>;
	Task(Func&& f, Callback cb, Args&&... args) :
		func_(std::forward<Func>(f)), args_(std::forward<Args>(args)...), callback_(std::move(cb)) {
	}
	void execute() override {
		callback_(std::apply(func_, args_));
	}

private:
	Func func_;
	std::tuple<Args...> args_;
	Callback callback_;
};