#include <cstdio>
#include <cstdlib>

#include "utils.h"

long long row_first(const int *data, int n) {
  long long sum = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      sum += data[row * n + col];
    }
  }
  return sum;
}

long long column_first(const int *data, int n) {
  long long sum = 0;
  for (int col = 0; col < n; col++) {
    for (int row = 0; row < n; row++) {
      sum += data[row * n + col];
    }
  }
  return sum;
}

int main(int argc, char **argv) {
  int n = 4096;
  if (argc > 1) {
    n = std::atoi(argv[1]);
  }
  if (n < 1 || n > 16384) {
    printf("Size must be between 1 and 16384.\n");
    return 1;
  }

  int *data = new int[n * n];
  for (int i = 0; i < n * n; i++) {
    data[i] = i % 100;
  }

  printf("%d x %d   %.3f MiB\n", n, n,
         n * n * sizeof(int) / (1024.0 * 1024.0));

  Timer timer;
  const int trials = 3;
  double row_seconds = 0;
  double column_seconds = 0;
  long long row_sum = 0;
  long long column_sum = 0;
  for (int trial = 0; trial < trials; trial++) {
    timer.start();
    row_sum += row_first(data, n);
    row_seconds += timer.stop();

    timer.start();
    column_sum += column_first(data, n);
    column_seconds += timer.stop();
  }

  row_seconds /= trials;
  column_seconds /= trials;
  printf("row: %.6f s   sum: %lld\n", row_seconds, row_sum / trials);
  printf("col: %.6f s   sum: %lld\n", column_seconds, column_sum / trials);
  printf("col / row: %.2f\n", column_seconds / row_seconds);

  delete[] data;
}
