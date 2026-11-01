// dot.cu
// Usage: ./dot [threads_per_block]
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "cuda_check.h"

__global__ void dot(const float *a, const float *b, float *partial, int n) {
  __shared__ float sums[1024];
  float sum = 0.0f;
  for (int i = blockIdx.x * blockDim.x + threadIdx.x; i < n;
       i += blockDim.x * gridDim.x) {
    sum += a[i] * b[i];
  }
  // TODO: add up the sums of this block and store the total in partial[blockIdx.x]
}

int main(int argc, char **argv) {
  int threads = argc > 1 ? atoi(argv[1]) : 256;
  int blocks = 256;
  int n = 1 << 24;
  int runs = 20;
  if (threads < 1 || threads > 1024) {
    printf("threads_per_block must be between 1 and 1024\n");
    return 1;
  }

  std::vector<float> a(n), b(n), partial(blocks);
  for (int i = 0; i < n; i++) {
    a[i] = i % 7;
  }

  float *d_a, *d_b, *d_partial;
  CUDA_CHECK(cudaMalloc(&d_a, n * sizeof(float)));
  CUDA_CHECK(cudaMalloc(&d_b, n * sizeof(float)));
  CUDA_CHECK(cudaMalloc(&d_partial, blocks * sizeof(float)));
  CUDA_CHECK(cudaMemcpy(d_a, a.data(), n * sizeof(float), cudaMemcpyHostToDevice));

  int wrong = 0;
  for (int r = 0; r < runs; r++) {
    // New values of b each run. A value left over from the last run is wrong.
    long long expected = 0;
    for (int i = 0; i < n; i++) {
      b[i] = (i + r) % 5;
      expected += (long long)(i % 7) * ((i + r) % 5);
    }
    CUDA_CHECK(cudaMemcpy(d_b, b.data(), n * sizeof(float), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemset(d_partial, 0, blocks * sizeof(float)));

    dot<<<blocks, threads>>>(d_a, d_b, d_partial, n);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaMemcpy(partial.data(), d_partial, blocks * sizeof(float),
                          cudaMemcpyDeviceToHost));

    double total = 0.0;
    for (int k = 0; k < blocks; k++) {
      total += partial[k];
    }
    if (total != expected) {
      if (wrong == 0) {
        printf("run %d: expected %lld, got %.0f\n", r, expected, total);
      }
      wrong++;
    }
  }

  printf("n = %d, %d blocks x %d threads\n", n, blocks, threads);
  printf("wrong: %d of %d runs\n", wrong, runs);

  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_partial);
  return 0;
}
