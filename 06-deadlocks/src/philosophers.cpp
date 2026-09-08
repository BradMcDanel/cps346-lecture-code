#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

const int NUM_PHILOSOPHERS = 3;
const int NUM_MEALS = 10;

std::mutex forks[NUM_PHILOSOPHERS];
int meals[NUM_PHILOSOPHERS] = {};

void think(int id) {
  std::this_thread::sleep_for(std::chrono::milliseconds(20 + id * 7));
}

void take_forks(int left_fork, int right_fork) {
  // TODO
}

void eat(int id) {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  meals[id]++;
}

void put_forks(int left_fork, int right_fork) {
  // TODO
}

void philosopher(int id) {
  int left_fork = id;
  int right_fork = (id + 1) % NUM_PHILOSOPHERS;

  for (int i = 0; i < NUM_MEALS; i++) {
    think(id);
    take_forks(left_fork, right_fork);
    eat(id);
    put_forks(left_fork, right_fork);
  }
}

int main() {
  std::vector<std::thread> threads;

  std::cout << "Starting philosophers..." << std::endl;

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    threads.push_back(std::thread(philosopher, i));
  }

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    threads[i].join();
  }

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    std::cout << "Philosopher " << i << ": " << meals[i] << " meals"
              << std::endl;
  }
}
