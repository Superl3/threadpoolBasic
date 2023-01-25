#include "Output.h"
#include "Calc.h"

Output::Output(OverallPerformanceMonitor* overall_performance_monitor_, std::string file_name_, bool isTest) : file_name(file_name_), overall_performance_monitor(overall_performance_monitor_){
	folder_path = "";

	if (isTest) {
		logging_enabled = true;
		console_enabled = false;
	}
}

Output::~Output() {
	if (write_file.is_open())
		write_file.close();
}

void Output::process(calcData* input,  int result, int duration) {

	overall_performance_monitor->addTask<std::chrono::milliseconds>(duration);

	auto single_line_text = toText(input, result, duration);

	if (logging_enabled)
		fileLogging(single_line_text);

	if (console_enabled)
		consoleWrite(single_line_text);
}

std::string Output::toText(calcData* input, const int& result, const int& duration) {
	return std::to_string(input->first) + "\t" + operatorToChar(input->op) + "\t" +  std::to_string(input->second) + "\t" + std::to_string(result) + "\t" + std::to_string(duration) + "\n";
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