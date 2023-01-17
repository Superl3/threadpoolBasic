#include "Output.h"

Output::Output() {
	folder_path = "";
	file_name = "test.txt";
}

Output::~Output() {
	if (write_file.is_open())
		write_file.close();
}

void Output::process(calcData& input, const int& result, const int& duration) {

	auto single_line_text = toText(input, result, duration);

	if (logging_enabled)
		fileLogging(single_line_text);

	if (console_enabled)
		consoleWrite(single_line_text);
}

std::string Output::toText(calcData& input, const int& result, const int& duration) {
	return std::to_string(input.first) + "\t" + input.op.toChar() + "\t" +  std::to_string(input.second) + "\t" + std::to_string(result) + "\t" + std::to_string(duration) + "\n";
}

#include<iostream>
void Output::consoleWrite(const std::string& line) {
	std::cout << line;
}

void Output::fileLogging(const std::string& line) {
	if (!write_file.is_open())
		write_file.open(file_name);
	write_file.write(line.c_str(), line.size());
}