cmake_minimum_required(VERSION 3.28)
project(ActiveObject)

set(CMAKE_CXX_STANDARD 20)

add_executable(ActiveObject main.cpp
        active_object.h
        active_object.cpp
)
