#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <vector>

#include "utils.h"

struct ThreadStats {
  int tasks = 0;
  long long cost = 0;
  long long checksum = 0;
  double seconds = 0.0;
};

void run_block(int n, int threads, std::vector<ThreadStats> &stats) {
  std::vector<std::thread> workers;
  int block = (n + threads - 1) / threads;
  for (int t = 0; t < threads; t++) {
    workers.emplace_back([&, t]() {
      ThreadStats local;
      Timer timer;
      timer.start();
      int start = std::min(n, t * block);
      int end = std::min(n, start + block);
      for (int i = start; i < end; i++) {
        local.checksum += work(i);
        local.cost += i;
        local.tasks++;
      }
      local.seconds = timer.stop();
      stats[t] = local;
    });
  }
  for (auto &worker : workers) {
    worker.join();
  }
}

void run_pool(int n, int threads, std::vector<ThreadStats> &stats) {
  // TODO: like run_block, but each thread claims its next task from a shared
  // std::atomic<int> counter until the tasks run out.
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atoi(argv[1]) : 2000;
  int threads = argc > 2 ? atoi(argv[2]) : 4;
  const char *mode = argc > 3 ? argv[3] : "block";

  if (n < 1 || threads < 1) {
    printf("Task and thread counts must be positive.\n");
    return 1;
  }

  std::vector<ThreadStats> stats(threads);
  Timer timer;
  timer.start();

  if (strcmp(mode, "block") == 0) {
    run_block(n, threads, stats);
  } else if (strcmp(mode, "pool") == 0) {
    run_pool(n, threads, stats);
  } else {
    printf("Mode must be block or pool.\n");
    return 1;
  }

  double seconds = timer.stop();
  long long checksum = 0;

  printf("mode: %s   threads: %d   tasks: %d\n", mode, threads, n);
  for (int t = 0; t < threads; t++) {
    printf("  thread %2d: %5d tasks   cost: %9lld   %.4f s\n", t,
           stats[t].tasks, stats[t].cost, stats[t].seconds);
    checksum += stats[t].checksum;
  }
  printf("time: %.4f s   checksum: %lld\n", seconds, checksum);
}
