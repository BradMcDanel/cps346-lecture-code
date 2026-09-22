#include "mnist.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>

uint32_t read_u32(std::ifstream &file) {
  unsigned char bytes[4];
  if (!file.read(reinterpret_cast<char *>(bytes), 4)) {
    throw std::runtime_error("Could not read an IDX header.");
  }
  return (uint32_t(bytes[0]) << 24) | (uint32_t(bytes[1]) << 16) |
         (uint32_t(bytes[2]) << 8) | uint32_t(bytes[3]);
}

Dataset load_mnist(const std::string &image_path,
                   const std::string &label_path, int limit) {
  std::ifstream images(image_path.c_str(), std::ios::binary);
  std::ifstream labels(label_path.c_str(), std::ios::binary);
  if (!images || !labels) {
    throw std::runtime_error("MNIST data is missing. Run python3 setup.py.");
  }

  uint32_t image_magic = read_u32(images);
  uint32_t image_count = read_u32(images);
  uint32_t rows = read_u32(images);
  uint32_t columns = read_u32(images);
  uint32_t label_magic = read_u32(labels);
  uint32_t label_count = read_u32(labels);

  if (image_magic != 2051 || label_magic != 2049 ||
      image_count != label_count || rows != MNIST_HEIGHT ||
      columns != MNIST_WIDTH) {
    throw std::runtime_error("The MNIST IDX headers are invalid.");
  }

  int count = static_cast<int>(image_count);
  if (limit > 0) {
    count = std::min(count, limit);
  }

  Dataset dataset;
  dataset.samples.resize(count);
  for (int i = 0; i < count; i++) {
    Sample &sample = dataset.samples[i];
    if (!labels.read(reinterpret_cast<char *>(&sample.label), 1) ||
        !images.read(reinterpret_cast<char *>(sample.pixels.data()),
                     MNIST_PIXELS)) {
      throw std::runtime_error("The MNIST IDX data ended early.");
    }
  }
  return dataset;
}

Dataset load_mnist_train(const std::string &data_dir, int limit) {
  return load_mnist(data_dir + "/train-images-idx3-ubyte",
                    data_dir + "/train-labels-idx1-ubyte", limit);
}

Dataset load_mnist_test(const std::string &data_dir, int limit) {
  return load_mnist(data_dir + "/t10k-images-idx3-ubyte",
                    data_dir + "/t10k-labels-idx1-ubyte", limit);
}

std::string find_mnist_data() {
  const char *directories[] = {"data", "../data"};
  for (const char *directory : directories) {
    std::ifstream images(std::string(directory) + "/train-images-idx3-ubyte",
                         std::ios::binary);
    if (images) {
      return directory;
    }
  }
  throw std::runtime_error("MNIST data is missing. Run python3 setup.py.");
}
