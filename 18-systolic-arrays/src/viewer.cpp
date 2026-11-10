// viewer.cpp
// Usage: ./viewer [M] [K] [N] [skew]
// Steps the systolic array one cycle at a time.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "raylib.h"
#include "systolic.h"

const Color INK = {40, 40, 40, 255};
const Color FAINT = {170, 170, 170, 255};
const Color WEIGHT = {244, 204, 204, 255};
const Color DATA = {207, 226, 243, 255};
const Color PARTIAL = {249, 203, 156, 255};
const Color RESULT = {255, 242, 204, 255};
const Color BUSY = {236, 246, 230, 255};
const Color GOOD = {56, 142, 60, 255};
const Color BAD = {211, 47, 47, 255};

Font font;

void text_center(const char *s, float cx, float cy, float size, Color color) {
  Vector2 dim = MeasureTextEx(font, s, size, 0);
  DrawTextEx(font, s, Vector2{cx - dim.x / 2, cy - dim.y / 2}, size, 0, color);
}

void text_left(const char *s, float x, float y, float size, Color color) {
  DrawTextEx(font, s, Vector2{x, y}, size, 0, color);
}

// A small labeled box (weight, data, or result)
void box(float cx, float cy, float w, float h, int value, Color fill, Color edge) {
  Rectangle rec = {cx - w / 2, cy - h / 2, w, h};
  DrawRectangleRec(rec, fill);
  DrawRectangleLinesEx(rec, 1.5f, edge);
  char s[16];
  snprintf(s, sizeof(s), "%d", value);
  text_center(s, cx, cy, h * 0.6f, INK);
}

void arrow(Vector2 a, Vector2 b, float head) {
  DrawLineEx(a, b, 2, INK);
  float dx = b.x - a.x, dy = b.y - a.y;
  float len = std::max(1.0f, std::abs(dx) + std::abs(dy));
  dx /= len;
  dy /= len;
  Vector2 l = {b.x - dx * head - dy * head * 0.6f, b.y - dy * head + dx * head * 0.6f};
  Vector2 r = {b.x - dx * head + dy * head * 0.6f, b.y - dy * head - dx * head * 0.6f};
  DrawTriangle(b, l, r, INK);
  DrawTriangle(b, r, l, INK);
}

// Draws a matrix of numbers with its top-left corner at (x, y)
void matrix(const char *name, float x, float y, float u, int rows, int cols,
            const std::vector<int> &v, Color fill) {
  text_left(name, x, y, u * 0.7f, INK);
  y += u;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      box(x + c * u + u / 2, y + r * u + u / 2, u, u, v[r * cols + c], fill, FAINT);
    }
  }
}

void draw(const Sim &sim, long long busy_total) {
  int sw = GetScreenWidth(), sh = GetScreenHeight();
  int M = sim.M, K = sim.K, N = sim.N;
  float bar = 50;

  // Top bar
  char s[256];
  long long slots = (long long)M * K * std::max(1, sim.cycle);
  snprintf(s, sizeof(s), "cycle %d    busy cells %d of %d    utilization %.0f%%    skew %d",
           sim.cycle, sim.busy(), M * K,
           100.0 * busy_total / slots, sim.skew);
  text_left(s, 20, 14, 24, INK);
  if (sim.done()) {
    int wrong = sim.wrong();
    if (wrong == 0) {
      snprintf(s, sizeof(s), "check: ok");
    } else {
      snprintf(s, sizeof(s), "check: %d of %d wrong", wrong, M * N);
    }
    Vector2 dim = MeasureTextEx(font, s, 24, 0);
    text_left(s, sw - dim.x - 20, 14, 24, wrong == 0 ? GOOD : BAD);
  }
  text_left("space: step   backspace: back   p: play   up/down: speed   r: reset",
            20, sh - 30, 18, FAINT);

  // Left panel: W, D, and R filling in as results leave the array
  int widest = std::max(K, N);
  int tallest = M + K + M + 3;
  float u = std::min(std::min(sw * 0.25f / (widest + 1), (sh - bar - 60) / (tallest + 3.0f)), 40.0f);
  float panel = 20 + (widest + 1) * u;
  float px = 20, py = bar + 10;
  matrix("W", px, py, u, M, K, sim.W, WEIGHT);
  py += (M + 1.5f) * u;
  matrix("D", px, py, u, K, N, sim.D, DATA);
  py += (K + 1.5f) * u;
  text_left("R = W x D", px, py, u * 0.7f, INK);
  py += u;
  for (int r = 0; r < M; r++) {
    for (int j = 0; j < N; j++) {
      float cx = px + j * u + u / 2, cy = py + r * u + u / 2;
      int want = sim.R[r * N + j];
      if (j < (int)sim.out[r].size()) {
        int got = sim.out[r][j];
        box(cx, cy, u, u, got, RESULT, got == want ? FAINT : BAD);
      } else {
        DrawRectangleLinesEx(Rectangle{cx - u / 2, cy - u / 2, u, u}, 1, FAINT);
      }
    }
  }

  // Fixed layout for the whole run: cells, the input queues below, and the
  // results to the right (N per row)
  float ax0 = panel + 30, ay0 = bar + 10;
  float aw = sw - ax0 - 20, ah = sh - ay0 - 50;
  int depth = std::max(sim.depth, 1);
  float cell = std::min(aw / (K * 1.6f + N * 0.68f + 0.2f),
                        ah / (M * 1.35f + depth * 0.36f + 0.2f));
  cell = std::min(cell, 160.0f);
  float gap = cell * 0.6f;    // room for the partial sum between columns
  float vgap = cell * 0.35f;  // data arrows between rows
  float pitch = cell + gap;
  float vpitch = cell + vgap;
  float b = cell * 0.38f;   // weight and data boxes
  float qb = cell * 0.32f;  // waiting inputs
  float top = ay0;
  float left = ax0;
  float bottom = top + M * vpitch - vgap;
  float right = left + K * pitch - gap;

  for (int r = 0; r < M; r++) {
    for (int c = 0; c < K; c++) {
      const Cell &cl = sim.grid.at(r, c);
      float x = left + c * pitch;
      float y = top + r * vpitch;
      float head = cell * 0.12f;
      arrow(Vector2{x + cell / 2, y + vpitch - 2}, Vector2{x + cell / 2, y + cell}, head);
      arrow(Vector2{x + cell, y + cell / 2}, Vector2{x + pitch - 2, y + cell / 2}, head);

      Rectangle rec = {x, y, cell, cell};
      DrawRectangleRec(rec, cl.valid ? BUSY : WHITE);
      DrawRectangleLinesEx(rec, 2, INK);
      float by = y + cell * 0.66f;
      box(x + cell * 0.28f, by, b, b, cl.w, WEIGHT, FAINT);
      if (cl.valid) {
        box(x + cell * 0.72f, by, b, b, cl.x, DATA, FAINT);
        // What the cell just did: partial sum in + w * x
        char eq[48];
        snprintf(eq, sizeof(eq), "%d + %d*%d", cl.y - cl.w * cl.x, cl.w, cl.x);
        float size = cell * 0.2f;
        float wide = MeasureTextEx(font, eq, size, 0).x;
        if (wide > cell * 0.92f) size *= cell * 0.92f / wide;
        text_center(eq, x + cell / 2, y + cell * 0.22f, size, INK);

        // The partial sum leaving to the right
        Vector2 yc = {x + cell + gap / 2, y + cell / 2};
        DrawCircleV(yc, gap * 0.42f, PARTIAL);
        char ys[16];
        snprintf(ys, sizeof(ys), "%d", cl.y);
        text_center(ys, yc.x, yc.y, gap * 0.4f, INK);
      } else {
        float bx = x + cell * 0.72f;
        DrawRectangleLinesEx(Rectangle{bx - b / 2, by - b / 2, b, b}, 1, FAINT);
      }
    }
  }

  // Inputs waiting below each column (skewed)
  for (int c = 0; c < K; c++) {
    for (int t = sim.cycle; t < sim.cycle + depth; t++) {
      int v;
      if (!sim.entering(c, t, &v)) continue;
      float cx = left + c * pitch + cell / 2;
      float cy = bottom + vgap + (t - sim.cycle) * qb * 1.12f + qb / 2;
      box(cx, cy, qb, qb, v, DATA, FAINT);
    }
  }

  // Results that have left each row, newest next to the array
  for (int r = 0; r < M; r++) {
    int n = sim.out[r].size();
    float cy = top + r * vpitch + cell / 2;
    float w = cell * 0.62f;
    for (int j = 0; j < n; j++) {
      float cx = right + gap + (n - 1 - j) * (w + 4) + w / 2;
      bool ok = sim.out[r][j] == sim.R[r * N + j];
      box(cx, cy, w, b, sim.out[r][j], RESULT, ok ? FAINT : BAD);
    }
  }
}

int main(int argc, char **argv) {
  int M = argc > 1 ? atoi(argv[1]) : 4;
  int K = argc > 2 ? atoi(argv[2]) : 4;
  int N = argc > 3 ? atoi(argv[3]) : 4;
  int skew = argc > 4 ? std::max(0, atoi(argv[4])) : 1;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(1400, 850, "Systolic array");
  // Use most of the screen
  int mon = GetCurrentMonitor();
  int ww = GetMonitorWidth(mon) * 0.85f, wh = GetMonitorHeight(mon) * 0.85f;
  if (ww > 0 && wh > 0) {
    SetWindowSize(ww, wh);
    SetWindowPosition((GetMonitorWidth(mon) - ww) / 2, (GetMonitorHeight(mon) - wh) / 2);
  }
  SetTargetFPS(60);
  const char *path = DATA_DIR "/DejaVuSansMono-Bold.ttf";
  if (FileExists(path)) {
    font = LoadFontEx(path, 64, NULL, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
  } else {
    font = GetFontDefault();
  }

  // history[i] is the state after i cycles, so we can step back
  std::vector<Sim> history = {Sim(M, K, N, skew)};
  std::vector<long long> busy = {0};
  bool playing = false;
  float delay = 0.6f, timer = 0;

  while (!WindowShouldClose()) {
    bool forward = IsKeyPressed(KEY_SPACE) || IsKeyPressedRepeat(KEY_SPACE) ||
                   IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT);
    bool back = IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE) ||
                IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT);
    if (IsKeyPressed(KEY_P)) playing = !playing;
    if (IsKeyPressed(KEY_UP)) delay = std::max(0.05f, delay / 2);
    if (IsKeyPressed(KEY_DOWN)) delay = std::min(3.0f, delay * 2);
    if (IsKeyPressed(KEY_R)) {
      history.erase(history.begin() + 1, history.end());
      busy.erase(busy.begin() + 1, busy.end());
      playing = false;
    }
    if (playing) {
      timer += GetFrameTime();
      if (timer >= delay) {
        timer = 0;
        forward = true;
      }
    }
    if (forward && !history.back().done()) {
      Sim next = history.back();
      next.advance();
      busy.push_back(busy.back() + next.busy());
      history.push_back(next);
    }
    if (back && history.size() > 1) {
      history.pop_back();
      busy.pop_back();
    }
    if (history.back().done()) playing = false;

    BeginDrawing();
    ClearBackground(WHITE);
    draw(history.back(), busy.back());
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
