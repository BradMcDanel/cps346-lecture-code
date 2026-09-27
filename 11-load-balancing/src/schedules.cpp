#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <vector>

#include "utils.h"

const char *schedule_name(omp_sched_t kind) {
  switch (kind & ~omp_sched_monotonic) {
  case omp_sched_static:
    return "static";
  case omp_sched_dynamic:
    return "dynamic";
  case omp_sched_guided:
    return "guided";
  default:
    return "auto";
  }
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atoi(argv[1]) : 2000;
  int threads = omp_get_max_threads();
  std::vector<int> thread_tasks(threads, 0);
  std::vector<double> thread_seconds(threads, 0.0);
  long long checksum = 0;

  // Ask OpenMP which schedule the OMP_SCHEDULE environment variable picked,
  // e.g. OMP_SCHEDULE="static,1"
  omp_sched_t kind;
  int chunk;
  omp_get_schedule(&kind, &chunk);

  Timer timer;
  timer.start();

#pragma omp parallel reduction(+ : checksum)
  {
    int id = omp_get_thread_num();
    int tasks = 0;
    Timer local;
    local.start();

    // Split the iterations among the threads. schedule(runtime) uses the same
    // OMP_SCHEDULE setting read above. nowait lets each thread stop its timer
    // as soon as its own share is done instead of waiting for the others.
#pragma omp for schedule(runtime) nowait
    for (int i = 0; i < n; i++) {
      checksum += work(i);
      tasks++;
    }

    thread_tasks[id] = tasks;
    thread_seconds[id] = local.stop();
  }

  double seconds = timer.stop();

  printf("max threads: %d   tasks: %d   schedule: %s, %d\n", threads, n,
         schedule_name(kind), chunk);
  for (int t = 0; t < threads; t++) {
    printf("  thread %2d: %5d tasks   %.4f s\n", t, thread_tasks[t],
           thread_seconds[t]);
  }
  printf("time: %.4f s   checksum: %lld\n", seconds, checksum);
}
