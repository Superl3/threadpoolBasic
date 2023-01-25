#include "Utility.h"

#include "Input.h"
#include "Output.h"
#include "ThreadPoolManager.h"

#include<iostream>


int main(int argc, char* argv[]) {

	int thread_count = 10;
	if (argc >= 2) thread_count = atoi(argv[1]);

	size_t max_queue_size = 100000;
	if (argc >= 3) max_queue_size = atoi(argv[2]);

	std::string log_file_name = "defaultLog.txt";
	if (argc >= 4) log_file_name.assign(argv[3], strlen(argv[3]));

	int test_case_count = 10000;
	if (argc >= 5) test_case_count = atoi(argv[4]);

	int output_thread_count = 1;

	OverallPerformanceMonitor* test_time_checker = new OverallPerformanceMonitor();
	ThreadPoolManager* tpm = new ThreadPoolManager(thread_count, output_thread_count, max_queue_size);
	Output *output = new Output(test_time_checker, log_file_name, true);

	Input *input = new Input(test_time_checker, tpm, output, test_case_count);
	input->execute();

	while (!input->isDone());
	delete tpm;

	test_time_checker->setEndTimer();

	int duration;
	if (test_time_checker->getRunningTime(duration)) {
		std::cout << "TESTCASE COUNT : " << test_case_count << "\n" <<
			"AVERAGE TIME PER TASK : " << test_time_checker->getAverageRunningTime<timeStandard>() << "ms\n" <<
			"TOTAL RUNNING TIME : " << duration << "ms\n";
	}

	delete output;
	delete input;
}
