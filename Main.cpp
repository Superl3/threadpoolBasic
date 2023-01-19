#include <functional>
#include "Input.h"
#include "Output.h"

#include "Task.h"

#include "ThreadPoolManager.h"

#include <iostream>

int add(int a, int b) {
	return a + b;
}

int main(int argc, char* argv[]) {

	int threadCount = 100;
	if (argc >= 2) threadCount = atoi(argv[1]);

	int maxQueueSize = 100;
	if (argc >= 3) maxQueueSize = atoi(argv[2]);

	Task<int, int> t(add, 3, 5);
	//t.execute();

	ThreadPoolManager* tpm = new ThreadPoolManager(1);

	Output *output = new Output();

	Input input(tpm, output);
	input.execute();

	while (true) {};
}
