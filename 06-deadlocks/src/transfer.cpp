#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

struct Account {
  long balance = 1000000;
  std::mutex m;
};

void transfer(Account &from, Account &to, long amount) {
  std::lock_guard<std::mutex> l1(from.m);
  std::lock_guard<std::mutex> l2(to.m);
  from.balance -= amount;
  to.balance += amount;
}

void repeated_transfer(Account &from, Account &to, long amount) {
  for (int i = 0; i < 10000; i++) {
    transfer(from, to, amount);
  }
}

int main() {
  Account a, b;

  std::cout << "Starting transfers..." << std::endl;

  std::thread t1(repeated_transfer, std::ref(a), std::ref(b), 100);
  std::thread t2(repeated_transfer, std::ref(b), std::ref(a), 50);

  t1.join();
  t2.join();

  std::cout << "Finished: " << a.balance << ", " << b.balance
            << std::endl;
}
