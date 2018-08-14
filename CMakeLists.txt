cmake_minimum_required(VERSION 3.2.3)
project(numconvert)

set(CMAKE_C_STANDARD 99)

set(SRC_DIR src)
include_directories(${SRC_DIR})

set(SOURCE_FILES ${SRC_DIR}/main.c ${SRC_DIR}/version.h)

add_executable(numconvert ${SOURCE_FILES})
