#include <iostream>
#include <thread>
#include <vector>

void say_hello(int id) {
  std::cout << "Hello from thread " << id << std::endl;
}

int main() {
  std::vector<std::thread> threads;

  for (int i = 0; i < 4; i++) {
    threads.push_back(std::thread(say_hello, i));
  }

  for (int i = 0; i < 4; i++) {
    threads[i].join();
  }
}
