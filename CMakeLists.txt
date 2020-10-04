cmake_minimum_required(VERSION 3.10)

project(Chip8)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


# Project headers
include_directories(../include/)

# Project Sources
file(GLOB T_SOURCES ./src/*.cpp)

add_executable(chip8 ${T_SOURCES})
target_link_libraries(chip8 ${SDL2_LIBRARIES})