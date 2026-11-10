// step.cpp
// One clock tick of the systolic array.
#include "systolic.h"

void step(Grid &g, const int *x_in, const bool *x_valid) {
  // Every cell reads what its neighbors held at the end of the last tick
  Grid old = g;
  for (int r = 0; r < g.rows; r++) {
    for (int c = 0; c < g.cols; c++) {
      Cell &cell = g.at(r, c);
      bool bottom = r == g.rows - 1;
      cell.valid = bottom ? x_valid[c] : old.at(r + 1, c).valid;
      int x = bottom ? x_in[c] : old.at(r + 1, c).x;      // from below
      int y = c == 0 ? 0 : old.at(r, c - 1).y;            // from the left
      if (cell.valid) {
        cell.x = x;
        cell.y = y + cell.w * x;
      }
    }
  }
}
