cmake_minimum_required(VERSION 3.15...3.31)

project(logic-and-proofs
  VERSION 1.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# for debugging/profiling
# TODO: make different compilation profiles ? commands ? for realease & debug
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS} -Wall -Wreturn-type -pedantic -Wextra -Wconversion -Wall -Wextra -Wundef -Wnull-dereference -Wpedantic -pedantic-errors -Wnoexcept -Wuninitialized -Wunused -Wunused-parameter -Winit-self -Wconversion -Wuseless-cast -Wextra-semi -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override  -Wformat-signedness -Wno-format-zero-length -Wmissing-include-dirs -Wshift-overflow=2 -Walloc-zero -Walloca -Wsign-promo -Wconversion -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow -Wshadow=local -Wmultiple-inheritance -Wvirtual-inheritance -Wno-virtual-move-assign -Wunsafe-loop-optimizations -Wnormalized -Wpacked -Wredundant-decls -Wmismatched-tags -Wredundant-tags -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wzero-as-null-pointer-constant -Wconditionally-supported -Werror=pedantic -Wwrite-strings -Wmultiple-inheritance -Wdouble-promotion -Wpointer-arith -Wcast-align=strict -Wcast-qual -Wconversion -Wsign-conversion -Wimplicit-fallthrough=1 -Wmisleading-indentation -Wdangling-else -Wdate-time -Wformat=2 -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2 -Wswitch-default -Wswitch-enum -Wstrict-overflow=5 -Wstringop-overflow=4 -Warray-bounds=2 -Wattribute-alias=2 -Wcatch-value=2 -Wplacement-new=2 -Wtrampolines -Winvalid-imported-macros -Winvalid-imported-macros")
# -Wunused-const-variable=2 -Weffc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -flto")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pg") # for profiling

#add_library(game Parser.h Parser.cpp Util.h Util.cpp pieces/Knight.h pieces/Knight.cpp Board.h Board.cpp
#            pieces/King.h pieces/King.cpp pieces/bitpieces/Piece.h pieces/Piece.cpp
#            pieces/bitpieces/Pawn.h pieces/Pawn.cpp
#            pieces/Bishop.h pieces/Bishop.cpp pieces/Rook.h pieces/Rook.cpp
#            pieces/Queen.h pieces/Queen.cpp Game.h Game.cpp)

include_directories(.)

add_library(headers
            Parser.h Parser.cpp
            Definitions.h Definitions.cpp
            Board.h Board.cpp BitMove.h BitMove.cpp BitOps.h BitOps.cpp # BitIterator.h BitIterator.cpp
            bitpieces/Pawn.h bitpieces/Pawn.cpp bitpieces/Knight.h bitpieces/Knight.cpp
            bitpieces/Queen.h bitpieces/Queen.cpp bitpieces/King.h bitpieces/King.cpp
            bitpieces/Bishop.h bitpieces/Bishop.cpp bitpieces/Rook.h bitpieces/Rook.cpp)
# the next line allows us to not use .cpp files all the time
# set_target_properties(headers PROPERTIES LINKER_LANGUAGE CXX)

add_executable(logic-and-proofs main.cpp)
target_link_libraries(logic-and-proofs PRIVATE headers)
# add_executable(precalc precalc.cpp)

#TODO: ask prof about cpu capabilities avx..

# TESTING: perft results

### v<1.0 - at depth 5, only en passant are missing, but more at depth 6 ?
# without optimization flags
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       0.00013 160
# 2       400             0       0.0022  180
# 3       8902            0       0.049   180
# 4       197281          0       1.2     170
# 5       4865351         -258    28      170
## with -O2
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       2.6e-05 770
# 2       400             0       0.00029 1400
# 3       8902            0       0.0065  1400
# 4       197281          0       0.16    1300
# 5       4865351         -258    3.8     1300
## to -O3 -march=native -flto
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       2.7e-05 740
# 2       400             0       0.00027 1500
# 3       8902            0       0.0061  1500
# 4       197281          0       0.14    1400
# 5       4865351         -258    3.4     1400
### v1.0 - after adding special moves:
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       6.5e-05 310
# 2       400             0       0.00088 460
# 3       8902            0       0.018   480
# 4       197281          0       0.43    460
# 5       4865609         0       11      450
# 6       119060324       0       2.8e+02 430

### v1.0-commit after cc36b4ac - after moving to bitboard
## with -O2
# [INITIAL_POS] Comparison with Perft Results from chessprogramming.org:
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       0.00    5000.00
# 2       400             0       0.00    7692.31
# 3       8902            0       0.00    5852.73
# 4       197281          0       0.03    7547.38
# 5       4865609         0       0.84    5767.97
# 6       119060324       0       16.53   7203.18
# [FEN_POS_2] Comparison with Perft Results from chessprogramming.org:
# Depth   Calculated      Error   seconds Kpos/s
# 1       48              0       0.00    3428.57
# 2       2039            0       0.00    6104.79
# 3       97862           0       0.02    5173.23
# 4       4085603         0       0.64    6337.14
# 5       193690690       0       37.68   5140.29
## with -O3 -march=native(on zen 5 ayoub) -flto
# [INITIAL_POS] Comparison with Perft Results from chessprogramming.org:
# Depth   Calculated      Error   seconds Kpos/s
# 1       20              0       0.00    10000.00
# 2       400             0       0.00    21052.63
# 3       8902            0       0.00    23426.32
# 4       197281          0       0.01    24678.63
# 5       4865609         0       0.20    23823.00
# 6       119060324       0       4.86    24483.58
# 7       3195018407      -883453 136.74  23365.24
# [FEN_POS_2] Comparison with Perft Results from chessprogramming.org:
# Depth   Calculated      Error   seconds Kpos/s
# 1       48              0       0.00    9600.00
# 2       2039            0       0.00    20595.96
# 3       97862           0       0.00    24368.03
# 4       4085603         0       0.17    23825.54
# 5       193690690       0       8.08    23976.25