#include <chrono>
#include <cstdio>
#include <cstdlib>

static const int DEFAULT_LIMIT = 40000000;

bool is_prime(int n) {
  if (n < 2) {
    return false;
  }
  for (int d = 2; (long)d * d <= n; d++) {
    if (n % d == 0) {
      return false;
    }
  }
  return true;
}

int main(int argc, char **argv) {
  int limit = (argc > 1) ? atoi(argv[1]) : DEFAULT_LIMIT;

  auto start_time = std::chrono::high_resolution_clock::now();

  int count = 0;
  for (int n = 2; n < limit; n++) {
    if (is_prime(n)) {
      count++;
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  double seconds =
      std::chrono::duration<double>(end_time - start_time).count();

  printf("primes below %d: %d   time: %.3f s\n", limit, count, seconds);

  return 0;
}
