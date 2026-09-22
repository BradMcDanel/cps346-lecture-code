#include <cstdio>
#include <exception>

#include "mnist.h"

int main() {
  try {
    std::string data_dir = find_mnist_data();
    Dataset train = load_mnist_train(data_dir);
    Dataset test = load_mnist_test(data_dir);

    printf("training samples: %zu\n", train.samples.size());
    printf("test samples:     %zu\n", test.samples.size());
    printf("image size:       %d x %d = %d pixels\n", MNIST_WIDTH,
           MNIST_HEIGHT, MNIST_PIXELS);
  } catch (const std::exception &error) {
    printf("%s\n", error.what());
    return 1;
  }
}
