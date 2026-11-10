// systolic.h
// Weight-stationary systolic array computing R = W x D.
// Cell (r, c) holds W[r][c], laid out like W (row 0 on top).
// Data enters at the bottom row and moves up. Partial sums move right.
#pragma once

#include <vector>

struct Cell {
  int w = 0;           // weight (never moves)
  int x = 0;           // data value
  int y = 0;           // partial sum
  bool valid = false;  // x and y hold real values this cycle
};

struct Grid {
  int rows, cols;
  std::vector<Cell> cells;

  Grid(int rows, int cols) : rows(rows), cols(cols), cells(rows * cols) {}
  Cell &at(int r, int c) { return cells[r * cols + c]; }
  const Cell &at(int r, int c) const { return cells[r * cols + c]; }
};

// One clock tick. x_in[c] / x_valid[c] is what enters column c from below.
void step(Grid &g, const int *x_in, const bool *x_valid);

struct Sim {
  int M, K, N;           // W is M x K, D is K x N, R is M x N
  int skew;              // column c starts skew * c cycles late
  std::vector<int> W, D;
  std::vector<int> R;    // expected answer (M x N)
  Grid grid;
  int cycle = 0;
  int depth = 0;       // cycles until the last value of D has entered
  int max_cycles = 0;  // give up after this many cycles
  std::vector<std::vector<int>> out;  // out[r] = values that left row r

  Sim(int M, int K, int N, int skew = 1, int seed = 1);
  bool entering(int c, int t, int *value) const;  // what enters column c at cycle t
  bool done() const;
  void advance();
  int busy() const;   // cells doing a multiply-add this cycle
  int wrong() const;  // results that are wrong or missing
};
