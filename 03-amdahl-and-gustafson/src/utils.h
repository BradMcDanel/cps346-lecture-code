// utils.h

#ifndef UTILS_H
#define UTILS_H

#include <chrono>

class Timer {
public:
  void start() { start_time = std::chrono::high_resolution_clock::now(); }

  double stop() {
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end_time - start_time).count();
  }

private:
  std::chrono::high_resolution_clock::time_point start_time;
};

#endif
