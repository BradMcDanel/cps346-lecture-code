// matmul.cu
// Usage: ./matmul [n]
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "cuda_check.h"

__global__ void matmul(const float *A, const float *B, float *C, int n) {
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;
  if (row < n && col < n) {
    float sum = 0.0f;
    for (int k = 0; k < n; k++) {
      sum += A[row * n + k] * B[k * n + col];
    }
    C[row * n + col] = sum;
  }
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atoi(argv[1]) : 1024;
  int runs = 5;
  size_t bytes = (size_t)n * n * sizeof(float);

  std::vector<float> A((size_t)n * n), B((size_t)n * n), C((size_t)n * n);
  for (size_t i = 0; i < A.size(); i++) {
    A[i] = rand() % 100 / 100.0f - 0.5f;
    B[i] = rand() % 100 / 100.0f - 0.5f;
  }

  float *d_A, *d_B, *d_C;
  CUDA_CHECK(cudaMalloc(&d_A, bytes));
  CUDA_CHECK(cudaMalloc(&d_B, bytes));
  CUDA_CHECK(cudaMalloc(&d_C, bytes));
  CUDA_CHECK(cudaMemcpy(d_A, A.data(), bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_B, B.data(), bytes, cudaMemcpyHostToDevice));

  dim3 threads(16, 16);
  dim3 blocks((n + 15) / 16, (n + 15) / 16);

  matmul<<<blocks, threads>>>(d_A, d_B, d_C, n);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaDeviceSynchronize());

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start);
  for (int r = 0; r < runs; r++) {
    matmul<<<blocks, threads>>>(d_A, d_B, d_C, n);
  }
  cudaEventRecord(stop);
  CUDA_CHECK(cudaEventSynchronize(stop));
  float total_ms;
  cudaEventElapsedTime(&total_ms, start, stop);
  float ms = total_ms / runs;

  CUDA_CHECK(cudaMemcpy(C.data(), d_C, bytes, cudaMemcpyDeviceToHost));

  int wrong = 0;
  for (int s = 0; s < 100; s++) {
    int row = rand() % n, col = rand() % n;
    double expected = 0.0;
    for (int k = 0; k < n; k++) {
      expected += (double)A[(size_t)row * n + k] * B[(size_t)k * n + col];
    }
    if (fabs(C[(size_t)row * n + col] - expected) > 1e-2) {
      wrong++;
    }
  }

  printf("n = %d, %d x %d blocks of 16 x 16 threads\n", n, blocks.x, blocks.y);
  printf("kernel: %.3f ms, %.1f GFLOP/s\n", ms, 2.0 * n * n * n / (ms * 1e6));
  if (wrong == 0) {
    printf("check: ok\n");
  } else {
    printf("check: %d of 100 sampled entries wrong\n", wrong);
  }

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);
  return 0;
}
