#include "TestCreator.h"

#include <random>

TestCreator::TestCreator(size_t test_size_input) : test_size(test_size_input) {
    createData();
}

TestCreator::~TestCreator() {
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

    test_data.clear();

    for (int i = 0; i < test_size; i++) {

        auto temp = createSample();

        calcData* data = new calcData();
        data->first = temp.first;
        data->second = temp.second;
        data->op = temp.op;

        test_data.emplace_back(data);
    }

}

bool TestCreator::getSingleTest(calcData* data) {
    if (test_data.empty()) return false;
    
    data = std::move(test_data.front());
    test_data.pop_front();

    return true;
}

std::deque<calcData*> TestCreator::getCreatedTest() {
    return test_data;
}