#include <stdio.h>
#include <stdlib.h>

__global__ void add_one(int *a, int n) {
  int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
  if (thread_id < n) { // Why is this necessary?
    a[thread_id] += 1;
  }
}

int main() {
  int n = 50;

  // Allocate memory on the host and initialize it
  int *a = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    a[i] = i;
  }

  // Allocate memory on the device
  int *d_a;
  cudaMalloc(&d_a, n * sizeof(int));
  cudaMemcpy(d_a, a, n * sizeof(int), cudaMemcpyHostToDevice);

  // Launch 2 blocks of 32 threads: 64 threads for 50 elements.
  add_one<<<2, 32>>>(d_a, n);

  // Copy result back to host
  cudaMemcpy(a, d_a, n * sizeof(int), cudaMemcpyDeviceToHost);

  // Free device memory
  cudaFree(d_a);

  // Print result
  for (int i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");

  // Free host memory
  free(a);

  return 0;
}
