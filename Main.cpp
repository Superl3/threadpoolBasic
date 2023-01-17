#include <functional>
#include "Input.h"
#include "Output.h"

int main(int argc, char* argv[]) {

  int threadCount = 100;
  if (argc >= 2) threadCount = atoi(argv[1]);

  int maxQueueSize = 100;
  if (argc >= 3) maxQueueSize = atoi(argv[2]);

  std::function<void(std::string)> callback = [](std::string a) {};

  //tpm.AddTask(calcData::calc, data, callback);
  /*ThreadPool pool(threadCount);

  std::mutex bufMutex;
  std::condition_variable mainCv;

  std::mutex outputMutex;
  std::condition_variable outputCv;

  std::thread outputThread(output, &outputMutex, &outputCv);

  outputThread.detach();
  inputThread.join();*/

  Input input;
  input.execute();

  Output output;
}
