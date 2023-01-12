bool isTestRunning = false;

void enqueueJob(ThreadPool::ThreadPool* pool, Buffer::calcData data, std::mutex* outputMutex, std::condition_variable* outputCv) {
    {
        std::lock_guard<std::mutex> lk(*outputMutex);
        outputList.push_back(pool->EnqueueJob(Buffer::calc, data));
    }
    outputCv->notify_all();
}

#include <random>
void doTest(ThreadPool::ThreadPool* pool, std::mutex* m,
    std::condition_variable* cv, std::mutex* outputMutex, std::condition_variable* outputCv) {

    std::mt19937 mtRand;
    auto createSample = [&mtRand]() -> Buffer::calcData {
        Buffer::calcData a{ mtRand() % 1000 + 1, mtRand() % 1000 + 1 };
        a.op = (OPERATOR::operators)(mtRand() % 4);
        return a;
    };

    while (isTestRunning) {
        enqueueJob(pool, createSample(), outputMutex, outputCv);
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

INPUTTYPE parsedInput(const std::string& buf, Buffer::calcData& parsedData) {
    auto splitData = [&buf]() -> std::vector<std::string> {
        int cur = 0, prev = 0;
        std::vector<std::string> splitedBuf;
        while (true) {
            cur = buf.find(' ', prev);
            if (cur == -1) {
                splitedBuf.push_back(buf.substr(prev));
                break;
            
            }
            splitedBuf.push_back(buf.substr(prev, cur - prev));
            prev = cur + 1;
        }
        return splitedBuf;
    };
    std::vector<std::string> splitedBuf = splitData();

    INPUTTYPE ret = INPUTTYPE::ERROR;

    if (splitedBuf.size() == 3) {
        auto isLegit = [](std::string a, std::string search) -> bool {
            return a.find_first_not_of(search) == std::string::npos;
        };
        std::string NUMBER = "0.123456789", OPERATORS = "+-*/";

        if (isLegit(splitedBuf[0], NUMBER) && isLegit(splitedBuf[1], OPERATORS) &&
            isLegit(splitedBuf[2], NUMBER) && splitedBuf[1].length() == 1) {
            parsedData.first = atoi(splitedBuf[0].c_str());
            parsedData.op = OPERATOR(splitedBuf[1][0]);
            parsedData.second = atoi(splitedBuf[2].c_str());
            ret = INPUTTYPE::CALC;
        }
    }
    else if (splitedBuf.size() == 1) {
        auto toLower = [](std::string s) -> std::string {
            std::transform(s.begin(), s.end(), s.begin(), tolower);
            return s;
        };

        if (toLower(splitedBuf[0]) == "test")
            ret = INPUTTYPE::TEST;
        else if (toLower(splitedBuf[0]) == "stoptest")
            ret = INPUTTYPE::STOPTEST;
        else if (toLower(splitedBuf[0]) == "exit" || splitedBuf[0] == "quit")
            ret = INPUTTYPE::QUIT;
    }
    if (ret != INPUTTYPE::ERROR) printf("%s\n", buf.c_str());
    return ret;
}

#include <conio.h>

void input(ThreadPool::ThreadPool* pool, std::mutex* m,
    std::condition_variable* cv, std::mutex* outputMutex, std::condition_variable* outputCv) {
    std::string inputBuffer = "";
    while (true) {
        char kbInput = _getch();
        if (kbInput == 13) {
            Buffer::calcData data;
            auto type = parsedInput(inputBuffer, data);
            switch (type) {
            case INPUTTYPE::CALC: {
                enqueueJob(pool, data, outputMutex, outputCv);
                break;
            }
            case INPUTTYPE::TEST: {
                if (!isTestRunning) {
                    isTestRunning = true;
                    std::thread testThread(doTest, pool, m, cv, outputMutex, outputCv);
                    testThread.detach();
                }
                break;
            }
            case INPUTTYPE::STOPTEST: {
                isTestRunning = false;
                break;
            }
            case INPUTTYPE::QUIT: {
                return;
            }
            }
            inputBuffer = "";
            continue;
        }
        inputBuffer += kbInput;
    }
}
