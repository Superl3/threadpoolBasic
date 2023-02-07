#pragma once

#include <deque>

#include "Calc.h"

class TestCreator {
public:
	TestCreator() {}
	~TestCreator() {}

	bool readData(const std::string& test_data_name);
	void createData(const size_t &test_size_);
	std::deque<calcData*> getCreatedTest();
	bool getSingleTest(calcData* singleData);

private:
	std::deque<calcData*> test_data;
};