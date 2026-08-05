// fetch_pages.cpp
//
// Downloads a batch of web pages, split evenly across the threads.
//
// We are not really hitting the network here. Each request is a
// sleep_for(100ms) instead, so that nobody has to install a networking
// library on the second day of class. This does not change the timing: to the
// operating system, a thread sitting in sleep_for and a thread sitting in
// recv() waiting on a web server are in the same state.
//
// Usage: ./fetch_pages <num_threads> [num_requests]

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

static const int DEFAULT_REQUESTS = 64;
static const int REQUEST_MS = 100;

// Fetch n pages, one after another.
void fetch(int n) {
  for (int i = 0; i < n; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(REQUEST_MS));
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <num_threads> [num_requests]\n", argv[0]);
    return 1;
  }

  int num_threads = atoi(argv[1]);
  int num_requests = (argc > 2) ? atoi(argv[2]) : DEFAULT_REQUESTS;

  if (num_threads < 1) {
    printf("num_threads must be at least 1\n");
    return 1;
  }

  std::vector<std::thread> threads;

  auto start_time = std::chrono::high_resolution_clock::now();

  for (int t = 0; t < num_threads; t++) {
    // Split the requests as evenly as possible across the threads. If there
    // are more threads than requests, the extra threads get nothing to do.
    int chunk = num_requests / num_threads;
    int remainder = num_requests % num_threads;
    int my_requests = chunk + (t < remainder ? 1 : 0);

    threads.emplace_back([my_requests]() { fetch(my_requests); });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  double seconds =
      std::chrono::duration<double>(end_time - start_time).count();

  printf("threads: %3d   requests: %d   time: %.4f s   "
         "(this machine reports %u cores)\n",
         num_threads, num_requests, seconds,
         std::thread::hardware_concurrency());

  return 0;
}
