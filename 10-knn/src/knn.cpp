#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <omp.h>
#include <vector>

#include "mnist.h"
#include "utils.h"

struct Neighbor {
  int distance;
  int label;
};

int squared_distance(const Sample &a, const Sample &b) {
  // TODO: compute the squared distance between two images.
  return 0;
}

int classify(const Sample &sample, const Dataset &training, int k) {
  // TODO: find the k nearest samples and return their majority label.
  return 0;
}

int main(int argc, char **argv) {
  int k = argc > 1 ? atoi(argv[1]) : 3;
  int train_count = argc > 2 ? atoi(argv[2]) : 5000;
  int test_count = argc > 3 ? atoi(argv[3]) : 200;

  try {
    if (k < 1 || train_count < 1 || test_count < 1) {
      printf("k and both sample counts must be positive.\n");
      return 1;
    }

    std::string data_dir = find_mnist_data();
    Dataset training = load_mnist_train(data_dir, train_count);
    Dataset test = load_mnist_test(data_dir, test_count);
    if (k > static_cast<int>(training.samples.size())) {
      printf("k must be between 1 and the training count.\n");
      return 1;
    }

    int correct = 0;
    Timer timer;
    timer.start();

    // TODO: classify every test sample and count the correct predictions.

    double seconds = timer.stop();
    double accuracy = 100.0 * correct / test.samples.size();

    printf("k: %d   training: %zu   test: %zu\n", k,
           training.samples.size(), test.samples.size());
    printf("max threads: %d   accuracy: %.2f%%   time: %.4f s\n",
           omp_get_max_threads(), accuracy, seconds);
  } catch (const std::exception &error) {
    printf("%s\n", error.what());
    return 1;
  }
}
