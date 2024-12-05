cmake_minimum_required(VERSION 3.15...3.31)

project(logic-and-proofs
  VERSION 0.1
  LANGUAGES CXX)

add_library(game Parser.h Parser.cpp Util.h Util.cpp 
            Board.h Board.cpp Square.h Square.cpp
            pieces/King.h pieces/King.cpp pieces/Piece.h pieces/Piece.cpp)
add_executable(logic-and-proofs main.cpp)
include_directories(.)
target_link_libraries(logic-and-proofs PRIVATE game)

