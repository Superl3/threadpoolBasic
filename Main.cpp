#include "Utility.h"

#include "Input.h"
#include "Output.h"
#include "ThreadPoolManager.h"

#include<iostream>

int main(int argc, char* argv[]) {


	int thread_count = THREAD_COUNT;
	if (argc >= 2) thread_count = atoi(argv[1]);
	//else {
	//	std::cout << "worker thread count (-1 = disable thread pool) : ";
	//	std::cin >> thread_count;
	//}

	int max_queue_size = -1;
	if (argc >= 3) max_queue_size = atoi(argv[2]);	
	//else {
	//	std::cout << "worker queue size (-1 = inf) : ";
	//	std::cin >> max_queue_size;
	//}

	std::string log_file_name = "none";
	if (argc >= 4) log_file_name.assign(argv[3], strlen(argv[3]));
	//else {
	//	std::cout << "log file name (none = skip) : ";
	//	std::cin >> log_file_name;
	//}

	int test_case_count = TEST_COUNT;
	if (argc >= 5) test_case_count = atoi(argv[4]);
	//else {
	//	std::cout << "test_case_count  : ";
	//	std::cin >> test_case_count;
	//}

	std::string test_file_name = TEST_FILE;
	TestCreator* tc = new TestCreator();

	if (test_file_name == "none") {
		tc->createData(test_case_count);
	}
	else {
		if (!tc->readData(test_file_name))
			tc->createData(test_case_count);
	}

	int output_thread_count = 1;

	ThreadPoolManager* tpm = new ThreadPoolManager(thread_count, output_thread_count, max_queue_size);
	Output *output = new Output(log_file_name);

	Input *input = new Input(tpm, tc, output, test_case_count);
	input->execute();
	while (!input->isDone())
	{
		std::this_thread::yield();
	}
	delete tpm;
	delete output;
	delete input;
}
