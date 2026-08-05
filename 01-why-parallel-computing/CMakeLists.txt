cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

# OpenMP is already wired up here so that adding a #pragma to the source is
# the only change needed. It is optional, so this still builds without it.
find_package(OpenMP)

add_executable(count_primes src/count_primes.cpp)
if(OpenMP_CXX_FOUND)
  target_link_libraries(count_primes PRIVATE OpenMP::OpenMP_CXX)
endif()
