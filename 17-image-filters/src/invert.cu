// invert.cu
// Usage: ./invert [image]
#include <stdio.h>
#include <stdlib.h>

#include "cuda_check.h"
#include "image.h"

void invert_cpu(const unsigned char *in, unsigned char *out, int n) {
  for (int i = 0; i < n; i++) {
    for (int c = 0; c < 3; c++) {
      out[i * 3 + c] = 255 - in[i * 3 + c];
    }
  }
}

__global__ void invert(const unsigned char *in, unsigned char *out, int n) {
  // TODO: one thread per pixel. Same work as one iteration of invert_cpu.
}

int main(int argc, char **argv) {
  const char *path = argc > 1 ? argv[1] : "data/mt-mitchell-nc.png";
  int w, h;
  unsigned char *in = load_image(path, &w, &h);
  int n = w * h;
  size_t bytes = (size_t)n * 3;
  unsigned char *out = (unsigned char *)malloc(bytes);
  unsigned char *expected = (unsigned char *)malloc(bytes);

  unsigned char *d_in, *d_out;
  CUDA_CHECK(cudaMalloc(&d_in, bytes));
  CUDA_CHECK(cudaMalloc(&d_out, bytes));
  CUDA_CHECK(cudaMemcpy(d_in, in, bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemset(d_out, 0, bytes));

  int threads = 256;
  int blocks = 1;  // TODO: enough blocks for n pixels
  invert<<<blocks, threads>>>(d_in, d_out, n);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaMemcpy(out, d_out, bytes, cudaMemcpyDeviceToHost));

  printf("%d x %d image, %d blocks x %d threads\n", w, h, blocks, threads);
  invert_cpu(in, expected, n);
  check(out, expected, w, h);
  write_image("invert.png", out, w, h);

  cudaFree(d_in);
  cudaFree(d_out);
  free(out);
  free(expected);
  stbi_image_free(in);
  return 0;
}
