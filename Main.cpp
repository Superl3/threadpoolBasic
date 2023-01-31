#include "Utility.h"

#include "Input.h"
#include "Output.h"
#include "ThreadPoolManager.h"

#include<iostream>


int main(int argc, char* argv[]) {

	int thread_count = 8;
	if (argc >= 2) thread_count = atoi(argv[1]);

	size_t max_queue_size = 10000;
	if (argc >= 3) max_queue_size = atoi(argv[2]);	

	std::string log_file_name = "defaultLog.txt";
	if (argc >= 4) log_file_name.assign(argv[3], strlen(argv[3]));

	int test_case_count = 1000000;
	if (argc >= 5) test_case_count = atoi(argv[4]);

	int output_thread_count = 10;

	ThreadPoolManager* tpm = new ThreadPoolManager(thread_count, output_thread_count, max_queue_size);
	Output *output = new Output(log_file_name);

	Input *input = new Input(tpm, output, test_case_count);
	input->execute();

	while (!input->isDone());

	delete tpm;
	delete output;
	delete input;
}
