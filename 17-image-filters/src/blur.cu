// blur.cu
// Usage: ./blur [radius] [image]
#include <chrono>
#include <stdio.h>
#include <stdlib.h>

#include "cuda_check.h"
#include "image.h"

void blur_cpu(const unsigned char *in, unsigned char *out, int w, int h, int radius) {
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int sum[3] = {0, 0, 0};
      int count = 0;
      for (int ny = y - radius; ny <= y + radius; ny++) {
        for (int nx = x - radius; nx <= x + radius; nx++) {
          if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
            for (int c = 0; c < 3; c++) {
              sum[c] += in[(ny * w + nx) * 3 + c];
            }
            count++;
          }
        }
      }
      for (int c = 0; c < 3; c++) {
        out[(y * w + x) * 3 + c] = sum[c] / count;
      }
    }
  }
}

__global__ void blur(const unsigned char *in, unsigned char *out, int w, int h,
                     int radius) {
  // TODO: one thread per pixel (x, y). Same work as one (x, y) of blur_cpu.
}

int main(int argc, char **argv) {
  int radius = argc > 1 ? atoi(argv[1]) : 4;
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
  dim3 blocks((w + 15) / 16, (h + 15) / 16);

  // Warm-up launch so the timed launch does not include CUDA startup.
  blur<<<blocks, threads>>>(d_in, d_out, w, h, radius);
  CUDA_CHECK(cudaDeviceSynchronize());

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start);
  blur<<<blocks, threads>>>(d_in, d_out, w, h, radius);
  cudaEventRecord(stop);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaEventSynchronize(stop));
  float gpu_ms;
  cudaEventElapsedTime(&gpu_ms, start, stop);
  CUDA_CHECK(cudaMemcpy(out, d_out, bytes, cudaMemcpyDeviceToHost));

  auto t0 = std::chrono::steady_clock::now();
  blur_cpu(in, expected, w, h, radius);
  auto t1 = std::chrono::steady_clock::now();
  double cpu_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

  printf("%d x %d image, radius %d (%d x %d box)\n", w, h, radius, 2 * radius + 1,
         2 * radius + 1);
  printf("cpu: %.1f ms, gpu kernel: %.2f ms\n", cpu_ms, gpu_ms);
  check(out, expected, w, h);
  write_image("blur.png", out, w, h);

  cudaFree(d_in);
  cudaFree(d_out);
  free(out);
  free(expected);
  stbi_image_free(in);
  return 0;
}
