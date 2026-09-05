#include <iostream>
#include <thread>
#include <vector>

long counter = 0;
const int N = 1000000;

void increment() {
  for (int i = 0; i < N; i++) {
    counter++;
  }
}

int main() {
  std::vector<std::thread> threads;

  for (int i = 0; i < 4; i++) {
    threads.push_back(std::thread(increment));
  }

  for (int i = 0; i < 4; i++) {
    threads[i].join();
  }

  std::cout << "expected: " << 4L * N << std::endl;
  std::cout << "actual:   " << counter << std::endl;
}
