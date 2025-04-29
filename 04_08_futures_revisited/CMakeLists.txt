cmake_minimum_required(VERSION 3.28)
project(futures)

set(CMAKE_CXX_STANDARD 17)

add_executable(futures main.cpp
        active_object.h
        future.h
        active_object.cpp)
