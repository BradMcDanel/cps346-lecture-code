// attention.cu
// Usage: ./attention [n]
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "attention.h"
#include "cuda_check.h"

// S = Q K^T: one thread per score
__global__ void scores(const float *Q, const float *K, float *S, int n) {
  int i = blockIdx.y * blockDim.y + threadIdx.y;
  int j = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n && j < n) {
    float s = 0.0f;
    for (int c = 0; c < D; c++) {
      s += Q[i * D + c] * K[j * D + c];
    }
    S[(size_t)i * n + j] = s;
  }
}

// Softmax of each row of S, in place: one block per row
__global__ void softmax(float *S, int n) {
  __shared__ float part[256];
  __shared__ float total;
  float *row = S + (size_t)blockIdx.x * n;

  float mine = 0.0f;
  for (int j = threadIdx.x; j < n; j += blockDim.x) {
    row[j] = expf(row[j]);
    mine += row[j];
  }
  part[threadIdx.x] = mine;
  __syncthreads();
  if (threadIdx.x == 0) {
    total = 0.0f;
    for (int t = 0; t < blockDim.x; t++) {
      total += part[t];
    }
  }
  __syncthreads();

  for (int j = threadIdx.x; j < n; j += blockDim.x) {
    row[j] /= total;
  }
}

// O = P V: one thread per output value
__global__ void output(const float *P, const float *V, float *O, int n) {
  int i = blockIdx.y * blockDim.y + threadIdx.y;
  int c = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n && c < D) {
    float o = 0.0f;
    for (int j = 0; j < n; j++) {
      o += P[(size_t)i * n + j] * V[j * D + c];
    }
    O[i * D + c] = o;
  }
}

void attention(const float *Q, const float *K, const float *V, float *S,
               float *O, int n) {
  dim3 threads(16, 16);
  scores<<<dim3(n / 16, n / 16), threads>>>(Q, K, S, n);
  softmax<<<n, 256>>>(S, n);
  output<<<dim3(D / 16, n / 16), threads>>>(S, V, O, n);
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atoi(argv[1]) : 16384;
  int runs = 3;
  if (n < 64 || n % 64 != 0) {
    printf("n must be a multiple of 64\n");
    return 1;
  }
  size_t bytes = (size_t)n * D * sizeof(float);
  size_t s_bytes = (size_t)n * n * sizeof(float);
  printf("n = %d, d = %d\n", n, D);
  printf("S: %.2f GB\n", s_bytes / 1073741824.0);

  std::vector<float> Q = random_rows(n, 1), K = random_rows(n, 2),
                     V = random_rows(n, 3), O((size_t)n * D);

  float *d_Q, *d_K, *d_V, *d_S, *d_O;
  CUDA_CHECK(cudaMalloc(&d_Q, bytes));
  CUDA_CHECK(cudaMalloc(&d_K, bytes));
  CUDA_CHECK(cudaMalloc(&d_V, bytes));
  CUDA_CHECK(cudaMalloc(&d_O, bytes));
  CUDA_CHECK(cudaMalloc(&d_S, s_bytes));
  CUDA_CHECK(cudaMemcpy(d_Q, Q.data(), bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_K, K.data(), bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(d_V, V.data(), bytes, cudaMemcpyHostToDevice));

  attention(d_Q, d_K, d_V, d_S, d_O, n);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaDeviceSynchronize());

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start);
  for (int r = 0; r < runs; r++) {
    attention(d_Q, d_K, d_V, d_S, d_O, n);
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
  cudaFree(d_S);
  cudaFree(d_O);
  return 0;
}
