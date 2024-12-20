cmake_minimum_required(VERSION 3.15...3.31)

project(logic-and-proofs
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pg") # for profiling

add_library(game Parser.h Parser.cpp Util.h Util.cpp pieces/Knight.h pieces/Knight.cpp Board.h Board.cpp
            pieces/King.h pieces/King.cpp pieces/Piece.h pieces/Piece.cpp
            pieces/Pawn.h pieces/Pawn.cpp
            pieces/Bishop.h pieces/Bishop.cpp pieces/Rook.h pieces/Rook.cpp
            pieces/Queen.h pieces/Queen.cpp Game.h Game.cpp)
add_executable(logic-and-proofs main.cpp)
include_directories(.)
target_link_libraries(logic-and-proofs PRIVATE game)

add_executable(precalc precalc.cpp)



# with flags
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       0.00013 1.6
# 2       400             0       0.0022  1.8
# 3       8902            0       0.049   1.8
# 4       197281          0       1.2     1.7
# 5       4865351         -258    28      1.7

# to -O2
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       2.6e-05 7.7
# 2       400             0       0.00029 14
# 3       8902            0       0.0065  14
# 4       197281          0       0.16    13
# 5       4865351         -258    3.8     13

# to -O3 -march=native -flto
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       2.7e-05 7.4
# 2       400             0       0.00027 15
# 3       8902            0       0.0061  15
# 4       197281          0       0.14    14
# 5       4865351         -258    3.4     14