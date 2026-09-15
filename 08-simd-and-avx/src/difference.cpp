#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "image.h"
#include "simd.h"
#include "utils.h"

float difference_scalar(const float *a, const float *b, float *output,
                        std::size_t n, float gain) {
  float total = 0.0f;
  for (std::size_t i = 0; i < n; i++) {
    float difference = a[i] - b[i];
    float error = difference * difference;
    output[i] = std::min(gain * error, 1.0f);
    total += error;
  }
  return total / n;
}

float difference_simd(const float *a, const float *b, float *output,
                      std::size_t n, float gain) {
  // TODO: replace the scalar version with SIMD intrinsics.
  return difference_scalar(a, b, output, n, gain);
}

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("usage: %s <a.bmp> <b.bmp> <output.bmp> [gain]\n", argv[0]);
    return 1;
  }

  float gain = argc > 4 ? std::atof(argv[4]) : 65536.0f;
  Image a;
  Image b;
  if (!load_bmp(argv[1], a) || !load_bmp(argv[2], b)) {
    printf("Could not read the input images.\n");
    return 1;
  }
  if (a.width != b.width || a.height != b.height) {
    printf("The input images must have the same dimensions.\n");
    return 1;
  }

  std::vector<float> expected(a.pixels.size());
  Image output = a;
  Timer timer;

  timer.start();
  float expected_error = difference_scalar(
      a.pixels.data(), b.pixels.data(), expected.data(), a.pixels.size(), gain);
  double scalar_seconds = timer.stop();

  timer.start();
  float actual_error =
      difference_simd(a.pixels.data(), b.pixels.data(), output.pixels.data(),
                      a.pixels.size(), gain);
  double simd_seconds = timer.stop();

  if (!save_bmp(argv[3], output)) {
    printf("Could not write %s.\n", argv[3]);
    return 1;
  }

  printf("%s   %d x %d\n", SIMD_NAME, a.width, a.height);
  printf("scalar: %.6f s   simd: %.6f s\n", scalar_seconds, simd_seconds);
  printf("mean squared error: %.9g\n", actual_error);
  printf("output max error: %.6g   score error: %.6g\n",
         max_error(expected.data(), output.pixels.data(), a.pixels.size()),
         std::fabs(expected_error - actual_error));
}
