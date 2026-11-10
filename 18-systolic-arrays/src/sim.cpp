// sim.cpp
// Feeds the array, calls step() once per cycle, and collects results.
#include <memory>
#include <random>

#include "systolic.h"

Sim::Sim(int M, int K, int N, int skew, int seed)
    : M(M), K(K), N(N), skew(skew), W(M * K), D(K * N), R(M * N, 0),
      grid(M, K), out(M) {
  std::mt19937 rng(seed);
  for (int &w : W) w = rng() % 4 + 1;
  for (int &d : D) d = rng() % 4 + 1;
  for (int r = 0; r < M; r++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < K; k++) {
        R[r * N + j] += W[r * K + k] * D[k * N + j];
      }
    }
  }
  for (int r = 0; r < M; r++) {
    for (int c = 0; c < K; c++) {
      grid.at(r, c).w = W[r * K + c];
    }
  }
  int v;
  for (int t = 0; t < 2 * (N + M + K) + skew * K; t++) {
    for (int c = 0; c < K; c++) {
      if (entering(c, t, &v)) depth = t + 1;
    }
  }
  max_cycles = depth + M + K + 2;
}

// Column c of the array gets row c of D, one value per cycle.
// Column c starts skew * c cycles after column 0.
bool Sim::entering(int c, int t, int *value) const {
  int j = t - skew * c;
  if (j < 0 || j >= N) return false;
  *value = D[c * N + j];
  return true;
}

bool Sim::done() const {
  if (cycle >= max_cycles) return true;
  for (int r = 0; r < M; r++) {
    if ((int)out[r].size() < N) return false;
  }
  return true;
}

void Sim::advance() {
  std::vector<int> x_in(K, 0);
  std::unique_ptr<bool[]> x_valid(new bool[K]);
  for (int c = 0; c < K; c++) {
    x_valid[c] = entering(c, cycle, &x_in[c]);
  }
  step(grid, x_in.data(), x_valid.get());
  for (int r = 0; r < M; r++) {
    const Cell &last = grid.at(r, K - 1);
    if (last.valid) out[r].push_back(last.y);
  }
  cycle++;
}

int Sim::busy() const {
  int n = 0;
  for (const Cell &cell : grid.cells) n += cell.valid;
  return n;
}

int Sim::wrong() const {
  int n = 0;
  for (int r = 0; r < M; r++) {
    for (int j = 0; j < N; j++) {
      if (j >= (int)out[r].size() || out[r][j] != R[r * N + j]) n++;
    }
  }
  return n;
}
