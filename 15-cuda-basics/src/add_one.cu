// add_one.cu
// Usage: ./add_one [blocks] [threads_per_block]
#include <stdio.h>
#include <stdlib.h>

#include "cuda_check.h"

__global__ void add_one(int *a, int n) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n) {
    a[i] += 1;
  }
}

int main(int argc, char **argv) {
  int blocks = argc > 1 ? atoi(argv[1]) : 2;
  int threads = argc > 2 ? atoi(argv[2]) : 32;
  int n = 50;

  int *a = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    a[i] = i;
  }

  int *d_a;
  CUDA_CHECK(cudaMalloc(&d_a, n * sizeof(int)));
  CUDA_CHECK(cudaMemcpy(d_a, a, n * sizeof(int), cudaMemcpyHostToDevice));

  add_one<<<blocks, threads>>>(d_a, n);
  CUDA_CHECK(cudaGetLastError());

  CUDA_CHECK(cudaMemcpy(a, d_a, n * sizeof(int), cudaMemcpyDeviceToHost));
  CUDA_CHECK(cudaFree(d_a));

  printf("%d blocks x %d threads = %d threads for %d elements\n", blocks,
         threads, blocks * threads, n);
  for (int i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  free(a);
  return 0;
}
