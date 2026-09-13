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

#endif
