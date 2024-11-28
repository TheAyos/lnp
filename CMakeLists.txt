cmake_minimum_required(VERSION 3.15...3.31)

project(logicAndProofs
  VERSION 0.1
  LANGUAGES CXX)

add_library(BoardH Board.cpp Board.hpp)
add_executable(main main.cpp)
include_directories(.)
target_link_libraries(main PRIVATE BoardH)