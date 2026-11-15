// flash.cu
// Usage: ./flash [n]
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "attention.h"
#include "cuda_check.h"

// One thread per query row. S is never stored.
__global__ void flash(const float *Q, const float *K, const float *V, float *O,
                      int n) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  float q[D], sum[D];
  for (int c = 0; c < D; c++) {
    q[c] = Q[i * D + c];
    sum[c] = 0.0f;
  }
  float total = 0.0f;

  for (int j = 0; j < n; j++) {
    float s = 0.0f;
    for (int c = 0; c < D; c++) {
      s += q[c] * K[j * D + c];
    }
    // TODO: update total and sum for key j
  }

  for (int c = 0; c < D; c++) {
    O[i * D + c] = sum[c] / total;
  }
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atoi(argv[1]) : 16384;
  int runs = 3;
  if (n < 64 || n % 64 != 0) {
    printf("n must be a multiple of 64\n");
    return 1;
  }
  size_t bytes = (size_t)n * D * sizeof(float);
  printf("n = %d, d = %d\n", n, D);

  std::vector<float> Q = random_rows(n, 1), K = random_rows(n, 2),
                     V = random_rows(n, 3), O((size_t)n * D);

  float *d_Q, *d_K, *d_V, *d_O;
  CUDA_CHECK(cudaMalloc(&d_Q, bytes));
  CUDA_CHECK(cudaMalloc(&d_K, bytes));
  CUDA_CHECK(cudaMalloc(&d_V, bytes));
  CUDA_CHECK(cudaMalloc(&d_O, bytes));
  CUDA_CHECK(cudaMemcpy(d_Q, Q.data(), bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_K, K.data(), bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_V, V.data(), bytes, cudaMemcpyHostToDevice));

  int threads = 64;
  flash<<<n / threads, threads>>>(d_Q, d_K, d_V, d_O, n);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaDeviceSynchronize());

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start);
  for (int r = 0; r < runs; r++) {
    flash<<<n / threads, threads>>>(d_Q, d_K, d_V, d_O, n);
  }
  cudaEventRecord(stop);
  CUDA_CHECK(cudaEventSynchronize(stop));
  float total_ms;
  cudaEventElapsedTime(&total_ms, start, stop);

  CUDA_CHECK(cudaMemcpy(O.data(), d_O, bytes, cudaMemcpyDeviceToHost));
  int wrong = check(Q, K, V, O, n);

  printf("kernel: %.3f ms\n", total_ms / runs);
  if (wrong == 0) {
    printf("check: ok\n");
  } else {
    printf("check: %d of 32 sampled rows wrong\n", wrong);
  }

  cudaFree(d_Q);
  cudaFree(d_K);
  cudaFree(d_V);
  cudaFree(d_O);
  return 0;
}
