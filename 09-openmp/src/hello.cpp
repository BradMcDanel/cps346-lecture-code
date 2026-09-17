#include <cstdio>
#include <omp.h>

int main() {
#pragma omp parallel
  {
    int id = omp_get_thread_num();
    int threads = omp_get_num_threads();
    printf("hello from thread %d of %d\n", id, threads);
  }
}
