#include "TestCreator.h"

#include <random>

TestCreator::TestCreator(size_t test_size_input) : test_size(test_size_input) {
    createData();
}

TestCreator::~TestCreator() {
    delete test_data;
    test_data = nullptr;
}

void TestCreator::createData() {    
    std::mt19937 mtRand;
    auto createSample = [&mtRand]() -> calcData {
        calcData sample{ mtRand() % 1000 + 1, mtRand() % 1000 + 1};
        int operator_key = mtRand() % 4;
        switch (operator_key) {
        case 0:
            sample.op = OPERATOR::PLUS;
            break;
        case 1:
            sample.op = OPERATOR::MINUS;
            break;
        case 2:
            sample.op = OPERATOR::MULTIPLY;
            break;
        case 3:
            sample.op = OPERATOR::DIVIDE;
            break;
        }
        return sample;
    };

    test_data = new std::deque<calcData>;
    for(int i = 0 ; i < test_size; i++)
        test_data->emplace_back(createSample());
}

bool TestCreator::getSingleTest(calcData* data) {
    if (test_data->empty()) return false;
    
    auto front = std::move(test_data->front());
    test_data->pop_front();

    data->op = front.op; 
    data->first = front.first;
    data->second = front.second;
    
    return true;
}

std::deque<calcData>* TestCreator::getCreatedTest() {
    return test_data;
}