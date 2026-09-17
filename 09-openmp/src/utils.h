#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <cstdint>

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

inline uint64_t mix(uint64_t x) {
  x += 0x9e3779b97f4a7c15ULL;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
  x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
  return x ^ (x >> 31);
}

inline double sample(uint64_t i) {
  return (mix(i) >> 11) * (1.0 / 9007199254740992.0);
}

#endif
