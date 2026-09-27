#ifndef UTILS_H
#define UTILS_H

#include <chrono>

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

// This just makes work proportional to units passed in
// The complicated sum with magic numbers is just so
// that a smart compiler can't optimize it away...
inline long long work(int units) {
  long long sum = 0;
  for (int i = 0; i < units * 400; i++) {
    sum += (i * 2654435761u) % 97;
  }
  return sum;
}

#endif
