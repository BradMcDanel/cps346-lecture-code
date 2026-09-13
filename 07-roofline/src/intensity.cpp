#include <cstdio>
#include <cstdlib>

#include "utils.h"

const int N = 32 * 1024 * 1024;

double compute(const double *data, int rounds) {
  double sum = 0;
  for (int i = 0; i < N; i++) {
    double x = data[i];
    for (int j = 0; j < rounds; j++) {
      x = x * 1.000001 + 0.000001;
    }
    sum += x;
  }
  return sum;
}

int main(int argc, char **argv) {
  int rounds = 1;
  if (argc > 1) {
    rounds = std::atoi(argv[1]);
  }
  if (rounds < 1 || rounds > 1024) {
    printf("Rounds must be between 1 and 1024.\n");
    return 1;
  }

  double *data = new double[N];
  for (int i = 0; i < N; i++) {
    data[i] = (i % 100) / 100.0;
  }

  Timer timer;
  const int trials = 3;
  double seconds = 0;
  double sum = 0;
  for (int trial = 0; trial < trials; trial++) {
    timer.start();
    sum += compute(data, rounds);
    seconds += timer.stop();
  }
  seconds /= trials;

  double bytes = N * sizeof(double);
  double flops = double(N) * (2 * rounds + 1);
  printf("rounds: %d   %.0f MiB\n", rounds, bytes / (1024 * 1024));
  printf("intensity: %.3f FLOP/byte\n", flops / bytes);
  printf("time: %.6f s   rate: %.3f GFLOP/s\n", seconds,
         flops / seconds / 1e9);
  printf("sum: %.6f\n", sum / trials);

  delete[] data;
}
