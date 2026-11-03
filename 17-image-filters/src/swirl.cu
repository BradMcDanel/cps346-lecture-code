// swirl.cu
// Usage: ./swirl [strength] [image]
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "cuda_check.h"
#include "image.h"

// Finds the source pixel (sx, sy) that lands on output pixel (x, y).
// __host__ __device__ lets both the CPU and the GPU call it.
__host__ __device__ void swirl_source(int x, int y, int w, int h, double strength,
                                      int *sx, int *sy) {
  double cx = w / 2.0, cy = h / 2.0;
  double dx = x - cx, dy = y - cy;
  double radius = 0.45 * h;
  double r = sqrt(dx * dx + dy * dy);
  double angle = 0.0;
  if (r < radius) {
    angle = strength * (1.0 - r / radius) * (1.0 - r / radius);
  }
  *sx = (int)floor(cx + dx * cos(angle) - dy * sin(angle));
  *sy = (int)floor(cy + dx * sin(angle) + dy * cos(angle));
}

void swirl_cpu(const unsigned char *in, unsigned char *out, int w, int h,
               double strength) {
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int sx, sy;
      swirl_source(x, y, w, h, strength, &sx, &sy);
      for (int c = 0; c < 3; c++) {
        if (sx >= 0 && sx < w && sy >= 0 && sy < h) {
          out[(y * w + x) * 3 + c] = in[(sy * w + sx) * 3 + c];
        } else {
          out[(y * w + x) * 3 + c] = 0;
        }
      }
    }
  }
}

__global__ void swirl(const unsigned char *in, unsigned char *out, int w, int h,
                      double strength) {
  // TODO: one thread per output pixel (x, y). Same work as one (x, y) of swirl_cpu.
}

int main(int argc, char **argv) {
  double strength = argc > 1 ? atof(argv[1]) : 4.0;
  int w, h;
  const char *path = argc > 2 ? argv[2] : "data/mt-mitchell-nc.png";
  unsigned char *in = load_image(path, &w, &h);
  size_t bytes = (size_t)w * h * 3;
  unsigned char *out = (unsigned char *)malloc(bytes);
  unsigned char *expected = (unsigned char *)malloc(bytes);

  unsigned char *d_in, *d_out;
  CUDA_CHECK(cudaMalloc(&d_in, bytes));
  CUDA_CHECK(cudaMalloc(&d_out, bytes));
  CUDA_CHECK(cudaMemcpy(d_in, in, bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemset(d_out, 0, bytes));

  dim3 threads(16, 16);
  dim3 blocks(1, 1);  // TODO: enough blocks to cover the w x h image
  swirl<<<blocks, threads>>>(d_in, d_out, w, h, strength);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaMemcpy(out, d_out, bytes, cudaMemcpyDeviceToHost));

  printf("%d x %d image, %d x %d blocks of 16 x 16 threads\n", w, h, blocks.x,
         blocks.y);
  swirl_cpu(in, expected, w, h, strength);
  check(out, expected, w, h);
  write_image("swirl.png", out, w, h);

  cudaFree(d_in);
  cudaFree(d_out);
  free(out);
  free(expected);
  stbi_image_free(in);
  return 0;
}
