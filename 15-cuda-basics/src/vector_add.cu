// vector_add.cu
// Usage: ./vector_add [n]
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "cuda_check.h"

__global__ void add(const float *a, const float *b, float *c, int n) {
  // TODO: c = a + b, one element per thread
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atoi(argv[1]) : 1000000;
  int threads = 256;
  int blocks = 1; // TODO: enough blocks to cover all n elements
  size_t bytes = (size_t)n * sizeof(float);

  std::vector<float> a(n), b(n), c(n), expected(n);
  for (int i = 0; i < n; i++) {
    a[i] = i % 1000;
    b[i] = i % 7;
  }

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < n; i++) {
    expected[i] = a[i] + b[i];
  }
  auto stop = std::chrono::steady_clock::now();
  double cpu_ms = std::chrono::duration<double, std::milli>(stop - start).count();

  float *d_a, *d_b, *d_c;
  CUDA_CHECK(cudaMalloc(&d_a, bytes));
  CUDA_CHECK(cudaMalloc(&d_b, bytes));
  CUDA_CHECK(cudaMalloc(&d_c, bytes));

  // The first launch pays a one-time setup cost. Keep it out of the timing.
  add<<<blocks, threads>>>(d_a, d_b, d_c, n);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaDeviceSynchronize());

  cudaEvent_t t0, t1, t2, t3;
  cudaEventCreate(&t0);
  cudaEventCreate(&t1);
  cudaEventCreate(&t2);
  cudaEventCreate(&t3);

  cudaEventRecord(t0);
  CUDA_CHECK(cudaMemcpy(d_a, a.data(), bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_b, b.data(), bytes, cudaMemcpyHostToDevice));
  cudaEventRecord(t1);
  add<<<blocks, threads>>>(d_a, d_b, d_c, n);
  CUDA_CHECK(cudaGetLastError());
  cudaEventRecord(t2);
  CUDA_CHECK(cudaMemcpy(c.data(), d_c, bytes, cudaMemcpyDeviceToHost));
  cudaEventRecord(t3);
  cudaEventSynchronize(t3);

  float to_gpu_ms, kernel_ms, to_cpu_ms;
  cudaEventElapsedTime(&to_gpu_ms, t0, t1);
  cudaEventElapsedTime(&kernel_ms, t1, t2);
  cudaEventElapsedTime(&to_cpu_ms, t2, t3);

  int wrong = 0;
  for (int i = 0; i < n; i++) {
    if (c[i] != expected[i]) {
      wrong++;
    }
  }

  printf("n = %d, %d blocks x %d threads\n", n, blocks, threads);
  printf("cpu loop:    %9.3f ms\n", cpu_ms);
  printf("copy to gpu: %9.3f ms\n", to_gpu_ms);
  printf("kernel:      %9.3f ms\n", kernel_ms);
  printf("copy to cpu: %9.3f ms\n", to_cpu_ms);
  printf("gpu total:   %9.3f ms\n", to_gpu_ms + kernel_ms + to_cpu_ms);
  if (wrong == 0) {
    printf("check: ok\n");
  } else {
    printf("check: %d of %d wrong\n", wrong, n);
  }

  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_c);
  return 0;
}
