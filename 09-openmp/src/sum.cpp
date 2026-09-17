#include <cstdio>
#include <cstdlib>
#include <omp.h>

#include "utils.h"

int main(int argc, char **argv) {
  long long n = argc > 1 ? atoll(argv[1]) : 10000000;
  long long sum = 0;

  Timer timer;
  timer.start();

  for (long long i = 1; i <= n; i++) {
    sum += i;
  }

  double seconds = timer.stop();
  long long expected = n * (n + 1) / 2;

  printf("max threads: %d\n", omp_get_max_threads());
  printf("sum: %lld   expected: %lld\n", sum, expected);
  printf("time: %.6f s\n", seconds);
}
