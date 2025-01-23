#pragma once

// #include "Evaluation.h"
#include "Board.h"
#include "BitMove.h"
// #include "MoveOrdering.h"
#include "PV.h"
// #include "TT.h"

#include <chrono>
#include <csetjmp>
#include <cstdlib>

namespace Search {
class SearchLimit {
public:
    SearchLimit(std::jmp_buf &jmpBuf, size_t searchTime);
    bool Reached();
    void Exit();

private:
    std::jmp_buf &_jmpBuf;
    std::chrono::steady_clock::time_point _endTime;
};

inline SearchLimit::SearchLimit(std::jmp_buf &jmpBuf, size_t searchTime)
    : _jmpBuf(jmpBuf),
      _endTime(std::chrono::steady_clock().now() + std::chrono::milliseconds(searchTime)) {}

inline bool SearchLimit::Reached() { return _endTime < std::chrono::steady_clock::now(); }

inline void SearchLimit::Exit() { longjmp(_jmpBuf, 1); }

namespace Functions {
int Quiesce(Board &board, int alpha, int beta, PV &pv);
int Negamax(Board &board, int alpha, int beta, int depth, int searchDepth, PV &pv, SearchLimit *limit = nullptr);
}; // namespace Functions

/*
BitMove BestMoveDepth(Board &board, int depth); */
BitMove BestMoveTime(Board &board, int time_limit);
}; // namespace Search
