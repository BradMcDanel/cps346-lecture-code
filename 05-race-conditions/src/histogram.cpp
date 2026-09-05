#include <cstdio>
#include <thread>
#include <vector>

#include "utils.h"

static const int NUM_BUCKETS = 16;
static const long NUM_VALUES = 40000000;

std::vector<unsigned char> data(NUM_VALUES);
long counts[NUM_BUCKETS] = {0};

void fill_data() {
  unsigned long x = 12345;
  for (long i = 0; i < NUM_VALUES; i++) {
    x = x * 6364136223846793005ULL + 1442695040888963407ULL;
    data[i] = (x >> 33) % NUM_BUCKETS;
  }
}

void histogram(long start, long end) {
  for (long i = start; i < end; i++) {
    counts[data[i]]++;
  }
}

int main() {
  fill_data();

  Timer timer;
  timer.start();

  // TODO: replace this call with four threads.
  histogram(0, NUM_VALUES);

  double seconds = timer.stop();

  long total = 0;
  for (int bucket = 0; bucket < NUM_BUCKETS; bucket++) {
    total += counts[bucket];
  }

  printf("time: %.3f s   total counted: %ld\n", seconds, total);
}
