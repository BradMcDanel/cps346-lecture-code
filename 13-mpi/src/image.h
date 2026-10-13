#ifndef IMAGE_H
#define IMAGE_H

#include <cstdio>
#include <vector>

class Image {
public:
  void add_row(const std::vector<double> &rod) { rows.push_back(rod); }

  void save(const char *path, int ranks) const {
    if (rows.empty()) {
      printf("no rows added, so %s was not written\n", path);
      return;
    }
    int width = (int)rows[0].size(), height = (int)rows.size();
    std::vector<bool> boundary(width, false);
    for (int r = 1; r < ranks; r++)
      boundary[r * (width / ranks) + (r < width % ranks ? r : width % ranks)] = true;
    int stride = (3 * width + 3) / 4 * 4;
    unsigned char header[54] = {'B', 'M'};
    put32(header + 2, 54 + stride * height);
    put32(header + 10, 54);
    put32(header + 14, 40);
    put32(header + 18, width);
    put32(header + 22, -height);
    header[26] = 1;
    header[28] = 24;
    std::vector<unsigned char> pixels(stride * height, 0);
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        unsigned char *p = &pixels[y * stride + 3 * x];
        if (boundary[x] && (y / 6) % 2 == 0)
          p[0] = p[1] = p[2] = 255;
        else
          color(rows[y][x], p);
      }
    }
    FILE *f = fopen(path, "wb");
    fwrite(header, 1, 54, f);
    fwrite(pixels.data(), 1, pixels.size(), f);
    fclose(f);
  }

private:
  std::vector<std::vector<double>> rows;

  static void put32(unsigned char *p, int v) {
    for (int i = 0; i < 4; i++)
      p[i] = (v >> (8 * i)) & 0xFF;
  }

  // 0 -> black, 1 -> yellow. BMP pixels are blue, green, red.
  static void color(double t, unsigned char *bgr) {
    static const unsigned char stops[8][3] = {
        {0, 0, 4},     {40, 11, 84},   {101, 21, 110}, {159, 42, 99},
        {212, 72, 66}, {245, 125, 21}, {250, 193, 39}, {252, 255, 164}};
    t = t < 0 ? 0 : (t > 1 ? 1 : t);
    double s = t * 7;
    int i = s >= 7 ? 6 : (int)s;
    double f = s - i;
    for (int c = 0; c < 3; c++)
      bgr[2 - c] = (unsigned char)(stops[i][c] + (stops[i + 1][c] - stops[i][c]) * f);
  }
};

#endif
