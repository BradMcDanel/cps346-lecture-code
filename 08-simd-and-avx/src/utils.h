#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <cmath>
#include <cstddef>

class Timer {
public:
  void start() { start_time = std::chrono::steady_clock::now(); }

  double stop() {
    auto end_time = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end_time - start_time).count();
  }

private:
  std::chrono::steady_clock::time_point start_time;
};

inline float max_error(const float *a, const float *b, std::size_t n) {
  float error = 0.0f;
  for (std::size_t i = 0; i < n; i++) {
    error = std::fmax(error, std::fabs(a[i] - b[i]));
  }
  return error;
}

#endif
