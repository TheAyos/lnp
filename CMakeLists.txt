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
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg") # for profiling
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# fastest, need to recompile on specific machine to run (march=native)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -flto")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,stack-size=10000000")

include_directories(.)

add_library(headers
            Parser.h Parser.cpp
            Definitions.h Definitions.cpp
            Board.h Board.cpp BitMove.h BitMove.cpp BitOps.h BitOps.cpp # BitIterator.h BitIterator.cpp
            Game.h Game.cpp
            bitpieces/Pawn.h bitpieces/Pawn.cpp bitpieces/Knight.h bitpieces/Knight.cpp
            bitpieces/Queen.h bitpieces/Queen.cpp bitpieces/King.h bitpieces/King.cpp
            bitpieces/Bishop.h bitpieces/Bishop.cpp bitpieces/Rook.h bitpieces/Rook.cpp
            Openings.h Openings.cpp
            Evaluator.h
            Evaluation.h)

add_executable(logic-and-proofs main.cpp)
target_link_libraries(logic-and-proofs PRIVATE headers)
