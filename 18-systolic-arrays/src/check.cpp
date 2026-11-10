// check.cpp
// Usage: ./check [M] [K] [N] [skew]
#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "systolic.h"

int main(int argc, char **argv) {
  int M = argc > 1 ? atoi(argv[1]) : 4;
  int K = argc > 2 ? atoi(argv[2]) : 4;
  int N = argc > 3 ? atoi(argv[3]) : 4;
  int skew = argc > 4 ? std::max(0, atoi(argv[4])) : 1;

  Sim sim(M, K, N, skew);
  long long busy = 0;
  while (!sim.done()) {
    sim.advance();
    busy += sim.busy();
  }

  long long slots = (long long)M * K * sim.cycle;
  printf("M = %d, K = %d, N = %d, skew = %d\n", M, K, N, skew);
  printf("cycles: %d  utilization: %.1f%%\n", sim.cycle, 100.0 * busy / slots);
  int wrong = sim.wrong();
  if (wrong == 0) {
    printf("check: ok\n");
  } else {
    printf("check: %d of %d results wrong\n", wrong, M * N);
  }
  return 0;
}
