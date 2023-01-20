#pragma once

#include<string>
#include<fstream>

struct calcData;

class Output {
public:
	Output(std::string file_name);
	~Output();
		
	void process(calcData *input, int result, int duration);

private:
	void consoleWrite(const std::string &line);
	void fileLogging(const std::string& line);

	std::string toText(calcData *input, const int& result, const int& duration);

	std::string filePath() {
		return folder_path + '\\' + file_name;
	}

	bool logging_enabled = true;
	bool console_enabled = true;

	std::string folder_path;
	std::string file_name;

	std::ofstream write_file;

	//std::mutex consoleWriteMutex;
};