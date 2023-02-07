#include "TestCreator.h"
#include "Utility.h"

#include <random>

bool TestCreator::readData(const std::string& test_file_name) {

    auto createCalcData = [](const int &a, const char& op, const int& b) -> calcData {

		calcData output;
		output.first = a;

		switch (op) {
		case '+':
			output.op = OPERATOR::PLUS;
			break;
		case '-':
			output.op = OPERATOR::MINUS;
			break;
		case '*':
			output.op = OPERATOR::MULTIPLY;
			break;
		case '/':
			output.op = OPERATOR::DIVIDE;
			break;
		}
		output.second = b;
		return output;
    };

    std::ifstream in(test_file_name);
    if (!in.is_open()) {
        return false;
    }

    int total = 0, success = 0;

    //std::string a, op, b, result, time;
    int a;
    char op;
    int b;
    int result;
    int time;
    do {
        in >> a >> op >> b >> result >> time;
        if (in.fail()) break;
        auto data = new calcData(createCalcData(a, op, b));

        test_data.emplace_back(data);
    } while (!in.eof());

    if (test_data.empty())
        return false;
    
    return true;
}

void TestCreator::createData(const size_t &test_size_) {    
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

    for (int i = 0; i < test_size_; i++) {

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