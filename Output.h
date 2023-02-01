#pragma once

#include<string>
#include<fstream>

#include "PerformanceMonitor.h"

struct calcData;

class Output {
public:
	Output(std::string file_name);
	~Output();
		
	void process(calcData *input, int result, int duration, bool isTest = false);

private:
	void consoleWrite(const std::string &line);
	void fileLogging(const std::string& line);

	std::string toText(calcData *input, const int& result, const int& duration);

	bool logging_enabled = true;
	bool console_enabled = true;

	const std::string file_name;

	std::ofstream write_file;

	//std::mutex consoleWriteMutex;
};