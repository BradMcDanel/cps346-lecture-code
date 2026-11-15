// attention.h
#pragma once

#include <math.h>
#include <stdlib.h>
#include <vector>

#define D 64

// n rows of D values in [-1, 1)
inline std::vector<float> random_rows(int n, unsigned seed) {
  srand(seed);
  std::vector<float> x((size_t)n * D);
  for (size_t i = 0; i < x.size(); i++) {
    x[i] = rand() % 2000 / 1000.0f - 1.0f;
  }
  return x;
}

// Recompute some rows of O on the CPU. Returns how many are wrong.
inline int check(const std::vector<float> &Q, const std::vector<float> &K,
                 const std::vector<float> &V, const std::vector<float> &O,
                 int n) {
  int wrong = 0;
  std::vector<double> s(n);
  for (int t = 0; t < 32; t++) {
    int i = rand() % n;
    double m = -INFINITY;
    for (int j = 0; j < n; j++) {
      s[j] = 0.0;
      for (int c = 0; c < D; c++) {
        s[j] += (double)Q[(size_t)i * D + c] * K[(size_t)j * D + c];
      }
      m = fmax(m, s[j]);
    }
    double l = 0.0;
    for (int j = 0; j < n; j++) {
      s[j] = exp(s[j] - m);
      l += s[j];
    }
    for (int c = 0; c < D; c++) {
      double o = 0.0;
      for (int j = 0; j < n; j++) {
        o += s[j] / l * V[(size_t)j * D + c];
      }
      if (!(fabs(O[(size_t)i * D + c] - o) < 1e-3)) {
        wrong++;
        break;
      }
    }
  }
  return wrong;
}
