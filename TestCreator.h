#pragma once

#include <deque>
#include "Buffer.h"

class TestCreator {
public:
	TestCreator(size_t test_size_input = 10);
	~TestCreator();

	void createData();
	std::deque<calcData>* getCreatedTest();
	bool getSingleTest(calcData& singleData);

private:
	std::deque<calcData>* test_data;
	const size_t test_size;
};