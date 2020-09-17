cmake_minimum_required (VERSION 2.8)

# Project Name
project (Chip-8)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 14)

# Project headers
include_directories(../include/)

# Project Sources
file(GLOB T_SOURCES ./*.cpp)

# Add gtest testing framework

add_subdirectory(../lib/gtest gtest_bin_dir)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(test ${T_SOURCES})

target_link_libraries(test gtest)