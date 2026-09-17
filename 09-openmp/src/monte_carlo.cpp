#include <cstdio>
#include <cstdlib>
#include <omp.h>

#include "utils.h"

int main(int argc, char **argv) {
  long long samples = argc > 1 ? atoll(argv[1]) : 10000000;
  long long inside = 0;

  Timer timer;
  timer.start();

  // Point i uses sample(2 * i) for x and sample(2 * i + 1) for y.
  // TODO: sample points and count those inside the quarter circle.

  double seconds = timer.stop();
  double pi = 4.0 * inside / samples;

  printf("max threads: %d   samples: %lld\n", omp_get_max_threads(), samples);
  printf("pi: %.8f   time: %.6f s\n", pi, seconds);
}
