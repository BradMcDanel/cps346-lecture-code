// collatz.cpp
//
// Usage: ./collatz <num_threads> <limit>

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

#include "utils.h"

// TODO: return the number of steps to reach 1.
// 6 -> 3 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1, so collatz(6) is 8.
int collatz(long n) {
  return 0;
}

// TODO: This function should call collatz in a loop from start to end (think about boundary conditions)
void longest_in_range(std::vector<long> &best_n, std::vector<int> &best_steps,
                      int t, long start, long end) {
  // t is the thread index
  // best_n and best_steps are parallel arrays
  // let's say we had 4 threads
  // best_n = [x, x, x, x]
  // for thread 2, t=2 and we write into best_n[t] and best_steps[t] when
  // we find a new longer number of steps.
  // so for 6, we would write:
  //   best_n[t] = 6
  //   best_steps[t] = 8
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("usage: %s <num_threads> <limit>\n", argv[0]);
    return 1;
  }

  int num_threads = atoi(argv[1]);
  long limit = atol(argv[2]);

  if (num_threads < 1) {
    printf("num_threads must be at least 1\n");
    return 1;
  }

  std::vector<long> best_n(num_threads, 0);
  std::vector<int> best_steps(num_threads, 0);

  Timer timer;
  timer.start();

  // TODO: Make threads in a loop a divide up the range and call longest_in_range!
  // Normally, it is a good idea to start by just calling longest_in_range serially and keep n small to start

  double seconds = timer.stop();

  long winner = 0;
  int steps = 0;
  for (int t = 0; t < num_threads; t++) {
    if (best_steps[t] > steps) {
      steps = best_steps[t];
      winner = best_n[t];
    }
  }

  printf("threads: %3d   limit: %ld   longest: %ld (%d steps)   time: %.4f s\n",
         num_threads, limit, winner, steps, seconds);

  return 0;
}
