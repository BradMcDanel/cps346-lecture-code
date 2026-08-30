#include <iostream>
#include <thread>

void say_hello(int id) {
  std::cout << "Hello from thread " << id << std::endl;
}

int main() {
  std::thread t(say_hello, 0);
  t.join();
}
