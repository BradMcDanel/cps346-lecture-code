#ifndef MNIST_H
#define MNIST_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

const int MNIST_WIDTH = 28;
const int MNIST_HEIGHT = 28;
const int MNIST_PIXELS = MNIST_WIDTH * MNIST_HEIGHT;
const int MNIST_CLASSES = 10;

struct Sample {
  std::array<uint8_t, MNIST_PIXELS> pixels;
  uint8_t label;
};

struct Dataset {
  std::vector<Sample> samples;
};

Dataset load_mnist(const std::string &image_path,
                   const std::string &label_path, int limit = 0);
Dataset load_mnist_train(const std::string &data_dir, int limit = 0);
Dataset load_mnist_test(const std::string &data_dir, int limit = 0);
std::string find_mnist_data();

#endif
