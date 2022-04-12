cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(tema_lab_2)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(tema_lab_2 ${SRC_FILES} main.cpp)

target_include_directories(tema_lab_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)