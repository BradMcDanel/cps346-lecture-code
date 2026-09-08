cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(transfer src/transfer.cpp)
target_link_libraries(transfer PRIVATE Threads::Threads)

add_executable(philosophers src/philosophers.cpp)
target_link_libraries(philosophers PRIVATE Threads::Threads)
