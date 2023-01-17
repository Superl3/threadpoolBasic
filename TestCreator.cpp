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
        calcData a{ mtRand() % 1000 + 1, mtRand() % 1000 + 1, Operator::fromRandom(mtRand() % 4) };
        return a;
    };

    test_data = new std::deque<calcData>;
    for(int i = 0 ; i < test_size; i++)
        test_data->emplace_back(createSample());
}

bool TestCreator::getSingleTest(calcData& data) {
    if (test_data->empty()) return false;
    
    data = std::move(test_data->front());
    test_data->pop_front();
    return true;
}

std::deque<calcData>* TestCreator::getCreatedTest() {
    return test_data;
}