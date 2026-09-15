#include <cstdio>
#include <vector>

#include "simd.h"
#include "utils.h"

void add_scalar(const float *a, const float *b, float *c, int n) {
  for (int i = 0; i < n; i++) {
    c[i] = a[i] + b[i];
  }
}

void add_simd(const float *a, const float *b, float *c, int n) {
  // TODO: replace the scalar version with SIMD intrinsics.
  add_scalar(a, b, c, n);
}

int main() {
  const int n = 16;
  std::vector<float> a(n);
  std::vector<float> b(n);
  std::vector<float> expected(n);
  std::vector<float> actual(n);

  for (int i = 0; i < n; i++) {
    a[i] = i;
    b[i] = 100 - 2 * i;
  }

  add_scalar(a.data(), b.data(), expected.data(), n);
  add_simd(a.data(), b.data(), actual.data(), n);

  for (int i = 0; i < n; i++) {
    printf("%5.1f + %5.1f = %5.1f\n", a[i], b[i], actual[i]);
  }
  printf("%s   max error: %.6g\n", SIMD_NAME,
         max_error(expected.data(), actual.data(), n));
}
