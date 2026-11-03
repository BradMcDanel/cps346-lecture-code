// pixelate.cu
// Usage: ./pixelate [tile] [image]
#include <stdio.h>
#include <stdlib.h>

#include "cuda_check.h"
#include "image.h"

void pixelate_cpu(const unsigned char *in, unsigned char *out, int w, int h, int tile) {
  for (int ty = 0; ty < h; ty += tile) {
    for (int tx = 0; tx < w; tx += tile) {
      int sum[3] = {0, 0, 0};
      int count = 0;
      for (int y = ty; y < ty + tile && y < h; y++) {
        for (int x = tx; x < tx + tile && x < w; x++) {
          for (int c = 0; c < 3; c++) {
            sum[c] += in[(y * w + x) * 3 + c];
          }
          count++;
        }
      }
      for (int y = ty; y < ty + tile && y < h; y++) {
        for (int x = tx; x < tx + tile && x < w; x++) {
          for (int c = 0; c < 3; c++) {
            out[(y * w + x) * 3 + c] = sum[c] / count;
          }
        }
      }
    }
  }
}

// One block per tile. Launch with tile x tile threads (at most 32 x 32).
__global__ void pixelate(const unsigned char *in, unsigned char *out, int w, int h) {
  __shared__ int sums[32][32][3];
  __shared__ int avg[3];
  int tx = threadIdx.x, ty = threadIdx.y;
  int x = blockIdx.x * blockDim.x + tx;
  int y = blockIdx.y * blockDim.y + ty;

  // TODO: copy this pixel into sums[ty][tx] (zeros if (x, y) is outside the image)

  // TODO: thread (0, 0) adds up the pixels of the tile and stores the average in avg

  // TODO: every thread writes avg to its pixel
}

int main(int argc, char **argv) {
  int tile = argc > 1 ? atoi(argv[1]) : 16;
  if (tile < 1 || tile > 32) {
    printf("tile must be between 1 and 32\n");
    return 1;
  }
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

  dim3 threads(tile, tile);
  dim3 blocks((w + tile - 1) / tile, (h + tile - 1) / tile);
  pixelate<<<blocks, threads>>>(d_in, d_out, w, h);
  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaMemcpy(out, d_out, bytes, cudaMemcpyDeviceToHost));

  printf("%d x %d image, %d x %d blocks of %d x %d threads\n", w, h, blocks.x,
         blocks.y, tile, tile);
  pixelate_cpu(in, expected, w, h, tile);
  check(out, expected, w, h);
  write_image("pixelate.png", out, w, h);

  cudaFree(d_in);
  cudaFree(d_out);
  free(out);
  free(expected);
  stbi_image_free(in);
  return 0;
}
