// estimate_pi.cpp
//
// Estimates pi with numerical integration:
//
//     pi = integral from 0 to 1 of  4 / (1 + x^2)  dx
//
// We chop the interval into a large number of thin slices, add up the area of
// each slice, and split those slices evenly across the threads.
//
// Note that each thread keeps its running total in a local variable and only
// touches the shared results vector once, right at the end.
//
// Usage: ./estimate_pi <num_threads> [num_steps]

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

// Lower this if a run with 1 thread takes too long on your machine.
static const long DEFAULT_STEPS = 500000000L;

// Add up the slices of the curve from [start, end).
double partial_sum(long start, long end, double step) {
  double sum = 0.0;
  for (long i = start; i < end; i++) {
    double x = (i + 0.5) * step;
    sum += 4.0 / (1.0 + x * x);
  }
  return sum;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <num_threads> [num_steps]\n", argv[0]);
    return 1;
  }

  int num_threads = atoi(argv[1]);
  long num_steps = (argc > 2) ? atol(argv[2]) : DEFAULT_STEPS;

  if (num_threads < 1) {
    printf("num_threads must be at least 1\n");
    return 1;
  }

  double step = 1.0 / (double)num_steps;

  // Each thread writes its answer into its own slot.
  std::vector<double> results(num_threads, 0.0);
  std::vector<std::thread> threads;

  auto start_time = std::chrono::high_resolution_clock::now();

  for (int t = 0; t < num_threads; t++) {
    // Split the steps as evenly as possible across the threads.
    long chunk = num_steps / num_threads;
    long remainder = num_steps % num_threads;
    long start = t * chunk + (t < remainder ? t : remainder);
    long end = start + chunk + (t < remainder ? 1 : 0);

    threads.emplace_back([&results, t, start, end, step]() {
      results[t] = partial_sum(start, end, step);
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  double pi = 0.0;
  for (double r : results) {
    pi += r;
  }
  pi *= step;

  auto end_time = std::chrono::high_resolution_clock::now();
  double seconds =
      std::chrono::duration<double>(end_time - start_time).count();

  printf("threads: %3d   steps: %ld   pi: %.10f   time: %.4f s   "
         "(this machine reports %u cores)\n",
         num_threads, num_steps, pi, seconds,
         std::thread::hardware_concurrency());

  return 0;
}
