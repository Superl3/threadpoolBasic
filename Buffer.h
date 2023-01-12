#pragma once

#include <mutex>
#include <thread>
#include <string>
#include <list>
#include <future>

class OPERATOR {
 public:
  enum operators { PLUS = 0, MINUS, MULTIPLY, DIVIDE, NONE };
  OPERATOR() = default;
  OPERATOR(operators op) : op_(op) {}
  OPERATOR(char raw) {
    switch (raw) {
      case '+':
        op_ = operators::PLUS;
        break;
      case '-':
        op_ = operators::MINUS;
        break;
      case '/':
        op_ = operators::DIVIDE;
        break;
      case '*':
        op_ = operators::MULTIPLY;
        break;
    }
  }
  int calc(const int& a, const int& b) {
    int ret = 0;
    switch (op_) {
      case operators::PLUS:
        ret = a + b;
        break;
      case operators::MINUS:
        ret = a - b;
        break;
      case operators::DIVIDE:
        ret = a / b;
        break;
      case operators::MULTIPLY:
        ret = a * b;
        break;
    }
    return ret;
  }
  std::string toString() {
    std::string ret = "";
    switch (op_) {
      case operators::PLUS:
        ret = '+';
        break;
      case operators::MINUS:
        ret = '-';
        break;
      case operators::DIVIDE:
        ret = '/';
        break;
      case operators::MULTIPLY:
        ret = '*';
        break;
    }
    return ret;
  }

 private:
  operators op_ = operators::NONE;
};

enum class INPUTTYPE { CALC = 0, TEST, STOPTEST, QUIT, ERROR };

class Buffer {
 public:
  struct calcData {
    int first, second;
    OPERATOR op;

    std::string toString() {
      return std::to_string(first) + ' ' + op.toString() + ' ' +
             std::to_string(second) + " = " +
             std::to_string(op.calc(first, second));
    }
  };


  static std::string calc(Buffer::calcData& data) {
    //printf("%s\n", data.toString().c_str());
    return data.toString();
  }
};
static std::list<std::future<std::string>> outputList;