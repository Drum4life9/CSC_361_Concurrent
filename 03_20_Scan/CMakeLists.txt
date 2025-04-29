cmake_minimum_required(VERSION 3.30)
project(03_20_Scan)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(03_20_Scan
        main.cpp
    SScan.cpp
    SScan.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")