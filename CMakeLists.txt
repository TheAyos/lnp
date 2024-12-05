cmake_minimum_required(VERSION 3.15...3.31)

project(logic-and-proofs
  VERSION 0.1
  LANGUAGES CXX)

add_library(game Board.h Board.cpp Square.h Square.cpp King.h King.cpp Pieces.h Pieces.cpp)
add_executable(main main.cpp)
include_directories(.)
target_link_libraries(main PRIVATE game)

