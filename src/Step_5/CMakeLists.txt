cmake_minimum_required(VERSION 3.9)
project(dispatcher)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic -g)

add_executable(dispatcher main.cpp)
