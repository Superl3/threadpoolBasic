#pragma once

void output(std::mutex* outputMutex, std::condition_variable* outputCv) {
	while (true) {
		std::unique_lock<std::mutex> lk(*outputMutex);
		outputCv->wait(lk, [&] { return !outputList.empty(); });
		auto result = std::move(outputList.front());
		outputList.pop_front();
		lk.unlock();

		printf("%s\n", result.get().c_str());
	}
}