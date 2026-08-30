cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(one_thread src/one_thread.cpp)
target_link_libraries(one_thread PRIVATE Threads::Threads)

add_executable(many_threads src/many_threads.cpp)
target_link_libraries(many_threads PRIVATE Threads::Threads)

add_executable(collatz src/collatz.cpp)
target_link_libraries(collatz PRIVATE Threads::Threads)
