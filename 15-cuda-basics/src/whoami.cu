// whoami.cu
// Usage: ./whoami [blocks] [threads_per_block]
#include <stdio.h>
#include <stdlib.h>

__global__ void whoami() {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  printf("block %d, thread %d -> i = %d\n", blockIdx.x, threadIdx.x, i);
}

int main(int argc, char **argv) {
  int blocks = argc > 1 ? atoi(argv[1]) : 2;
  int threads = argc > 2 ? atoi(argv[2]) : 4;

  whoami<<<blocks, threads>>>();
  cudaDeviceSynchronize();

  printf("done\n");
  return 0;
}
