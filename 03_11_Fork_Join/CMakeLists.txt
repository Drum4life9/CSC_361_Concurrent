cmake_minimum_required(VERSION 3.23)
project(03_11_Fork_Join)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")

add_executable(03_11_Fork_Join main.cpp)