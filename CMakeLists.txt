cmake_minimum_required(VERSION 3.15...3.31)

project(logic-and-proofs
  VERSION 0.1
  LANGUAGES CXX)

add_library(BoardH Board.cpp Board.hpp)
add_executable(logic-and-proofs main.cpp)
include_directories(.)
target_link_libraries(logic-and-proofs PRIVATE BoardH)