cmake_minimum_required(VERSION 3.7)
project(numconvert)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/main.c)

add_executable(numconvert ${SOURCE_FILES})