cmake_minimum_required(VERSION 3.26)
project(03_11_Fork_Join)

# Specify the path to GCC-13 (replace with your actual path)

set(CMAKE_C_STANDARD 20)

# Find OpenMP package
find_package(OpenMP REQUIRED)

add_executable(03_11_Fork_Join main.cpp)

# Enable OpenMP support
if (OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    target_link_libraries(03_11_Fork_Join PRIVATE OpenMP::OpenMP_C)
endif ()