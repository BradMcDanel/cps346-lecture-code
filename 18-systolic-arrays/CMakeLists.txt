cmake_minimum_required(VERSION 3.14)
project(lecture-code C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wextra")

include_directories(include)

add_library(systolic src/step.cpp src/sim.cpp)
add_executable(check src/check.cpp)
target_link_libraries(check PRIVATE systolic)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

if(UNIX AND NOT APPLE AND NOT EXISTS /usr/include/X11/extensions/Xrandr.h)
  message(FATAL_ERROR "\nMissing packages for the viewer window. Run this once, then cmake again:\n"
    "  sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev\n")
endif()

# Downloads and builds raylib the first time you run cmake
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz)
FetchContent_MakeAvailable(raylib)
target_compile_options(raylib PRIVATE -w)
target_compile_options(glfw PRIVATE -w)

add_executable(viewer src/viewer.cpp)
target_compile_definitions(viewer PRIVATE DATA_DIR="${CMAKE_SOURCE_DIR}/data")
target_link_libraries(viewer PRIVATE systolic raylib)
