cmake_minimum_required(VERSION 3.30)
project(03_27_Zip)

set(CMAKE_CXX_STANDARD 20)

add_executable(03_27_Zip main.cpp)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fopenmp")