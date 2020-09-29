cmake_minimum_required (VERSION 2.8)

# Project Name
project (Chip-8)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 14)

# Project headers
include_directories(./include/)

# Project Sources
file(GLOB T_SOURCES ./src/*.cpp)

add_executable(chip8 ${T_SOURCES})