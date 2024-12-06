cmake_minimum_required(VERSION 3.15...3.31)

project(logic-and-proofs
  VERSION 0.1
  LANGUAGES CXX)

add_library(game Parser.h Parser.cpp Util.h Util.cpp pieces/Knight.h pieces/Knight.cpp Board.h Board.cpp
            pieces/King.h pieces/King.cpp pieces/Piece.h pieces/Piece.cpp
            pieces/Pawn.h pieces/Pawn.cpp
            pieces/Bishop.h pieces/Bishop.cpp pieces/Rook.h pieces/Rook.cpp
            pieces/Queen.h pieces/Queen.cpp Game.h Game.cpp)
add_executable(logic-and-proofs main.cpp)
include_directories(.)
target_link_libraries(logic-and-proofs PRIVATE game)

