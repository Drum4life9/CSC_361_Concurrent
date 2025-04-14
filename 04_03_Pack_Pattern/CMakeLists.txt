cmake_minimum_required(VERSION 3.30)
project(04_03_Pack_Pattern)

set(CMAKE_CXX_STANDARD 20)

add_executable(04_03_Pack_Pattern main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")