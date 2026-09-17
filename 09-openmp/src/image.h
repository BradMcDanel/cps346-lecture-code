#ifndef IMAGE_H
#define IMAGE_H

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

struct Image {
  int width;
  int height;
  std::vector<float> pixels;
};

inline uint16_t read_u16(const unsigned char *p) {
  return uint16_t(p[0]) | (uint16_t(p[1]) << 8);
}

inline uint32_t read_u32(const unsigned char *p) {
  return uint32_t(p[0]) | (uint32_t(p[1]) << 8) | (uint32_t(p[2]) << 16) |
         (uint32_t(p[3]) << 24);
}

inline void write_u16(unsigned char *p, uint16_t value) {
  p[0] = value & 0xff;
  p[1] = value >> 8;
}

inline void write_u32(unsigned char *p, uint32_t value) {
  p[0] = value & 0xff;
  p[1] = (value >> 8) & 0xff;
  p[2] = (value >> 16) & 0xff;
  p[3] = (value >> 24) & 0xff;
}

inline bool load_bmp(const std::string &path, Image &image) {
  std::ifstream file(path.c_str(), std::ios::binary);
  unsigned char header[54];
  if (!file.read(reinterpret_cast<char *>(header), sizeof(header))) {
    return false;
  }

  if (header[0] != 'B' || header[1] != 'M' || read_u32(header + 14) < 40 ||
      read_u16(header + 26) != 1 || read_u16(header + 28) != 24 ||
      read_u32(header + 30) != 0) {
    return false;
  }

  int32_t width = static_cast<int32_t>(read_u32(header + 18));
  int32_t signed_height = static_cast<int32_t>(read_u32(header + 22));
  if (width <= 0 || signed_height == 0) {
    return false;
  }

  image.width = width;
  image.height = signed_height < 0 ? -signed_height : signed_height;
  image.pixels.resize(std::size_t(image.width) * image.height * 3);

  std::size_t row_size = (std::size_t(image.width) * 3 + 3) & ~std::size_t(3);
  std::vector<unsigned char> row(row_size);
  file.seekg(read_u32(header + 10));

  for (int file_row = 0; file_row < image.height; file_row++) {
    if (!file.read(reinterpret_cast<char *>(row.data()), row.size())) {
      return false;
    }
    int y = signed_height > 0 ? image.height - 1 - file_row : file_row;
    for (int x = 0; x < image.width; x++) {
      std::size_t source = std::size_t(x) * 3;
      std::size_t target = (std::size_t(y) * image.width + x) * 3;
      image.pixels[target] = row[source + 2] / 255.0f;
      image.pixels[target + 1] = row[source + 1] / 255.0f;
      image.pixels[target + 2] = row[source] / 255.0f;
    }
  }
  return true;
}

inline bool save_bmp(const std::string &path, const Image &image) {
  std::size_t row_size = (std::size_t(image.width) * 3 + 3) & ~std::size_t(3);
  uint32_t data_size = row_size * image.height;
  unsigned char header[54] = {};
  header[0] = 'B';
  header[1] = 'M';
  write_u32(header + 2, 54 + data_size);
  write_u32(header + 10, 54);
  write_u32(header + 14, 40);
  write_u32(header + 18, image.width);
  write_u32(header + 22, image.height);
  write_u16(header + 26, 1);
  write_u16(header + 28, 24);
  write_u32(header + 34, data_size);

  std::ofstream file(path.c_str(), std::ios::binary);
  if (!file.write(reinterpret_cast<char *>(header), sizeof(header))) {
    return false;
  }

  std::vector<unsigned char> row(row_size, 0);
  for (int y = image.height - 1; y >= 0; y--) {
    for (int x = 0; x < image.width; x++) {
      std::size_t source = (std::size_t(y) * image.width + x) * 3;
      std::size_t target = std::size_t(x) * 3;
      float r = std::max(0.0f, std::min(1.0f, image.pixels[source]));
      float g = std::max(0.0f, std::min(1.0f, image.pixels[source + 1]));
      float b = std::max(0.0f, std::min(1.0f, image.pixels[source + 2]));
      row[target] = static_cast<unsigned char>(b * 255.0f + 0.5f);
      row[target + 1] = static_cast<unsigned char>(g * 255.0f + 0.5f);
      row[target + 2] = static_cast<unsigned char>(r * 255.0f + 0.5f);
    }
    if (!file.write(reinterpret_cast<char *>(row.data()), row.size())) {
      return false;
    }
  }
  return true;
}

#endif
