cmake_minimum_required(VERSION 3.20)
project(cyb3053_project2 C)

set(CMAKE_C_STANDARD 11)

# ✅ This line tells CMake where to find alloc.h
include_directories(include)

add_executable(cyb3053_project2
    src/main.c
    src/alloc.c
)
