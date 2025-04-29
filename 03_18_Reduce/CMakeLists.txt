cmake_minimum_required(VERSION 3.30)
project(03_18_Reduce)

set(CMAKE_CXX_STANDARD 17)

add_executable(03_18_Reduce main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
