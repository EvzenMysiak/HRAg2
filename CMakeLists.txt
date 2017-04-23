cmake_minimum_required(VERSION 3.7)
project(HRAg2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c variables.h)
add_executable(HRAg2 ${SOURCE_FILES})