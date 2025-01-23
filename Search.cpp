#include "Search.h"
#include "Evaluation.h"
#include "TT.h"
#include "MoveOrdering.h"

namespace Search {

namespace Functions {
namespace {
bool AB(int score, int &alpha, int &beta) {
    if (score >= beta) return true;
    if (score > alpha) alpha = score;
    return false;
}
} //namespace

int Quiesce(Board &board, int alpha, int beta, PV &pv) {
    int standPat = Evaluation::eval(board);
    if (AB(standPat, alpha, beta)) return beta;

    MoveList moves = board.get_capture_moves();
    MoveOrdering::MVV_LVA(board, moves);
    MoveOrdering::PV_P(board, pv, moves);
    for (auto move :moves) {
	BoardState savedState(board);
	board.make_move(move);
	/* if (board.playerInCheck(board.turn)) {
	    savedState.reapply(board);
	    continue;
	} */
	int score = -Quiesce(board, -beta, -alpha, pv);
	savedState.reapply(board);
	if (AB(score, alpha, beta)) return beta;
    }
    // std::cout << alpha << std::endl;
    return alpha;
}

int Negamax(Board &board, int alpha, int beta, int depth, int searchDepth, PV &pv, SearchLimit *limit) {
    std::array<BitMove, 512> killer_moves;
    if (searchDepth == 0)
	for (BitMove &move : killer_moves)
	    move = BitMove();

    if (limit != nullptr & depth > 3 && limit->Reached())
	limit->Exit();
    
    //threefold
    if (board.isThreefold())
	return 0;

    if (depth <= 0) {
	// return Evaluation::eval(board); 
	return Quiesce(board, alpha, beta, pv); 
    }
    

    U64 hash = board.get_hash();
    // std::cout << hash << std::endl;
    auto tt = TT::Probe(hash, depth, searchDepth, alpha, beta);

    if (tt.score != TT::probe_fail) return tt.score;

    int ttBound = TT::probe_upper;
    MoveList moves = board.get_all_legal_moves();
    
    if (moves.empty()) return Evaluation::evalnomove(board);
    
    if (BitMove killer_move = killer_moves[searchDepth]; killer_move.is_defined())
        MoveOrdering::Killer(moves, killer_move);
    MoveOrdering::All(board, tt.move, pv, moves);
    BitMove bm = moves[0];
    

    for (size_t i = 0; i < moves.size(); i++) {
	BitMove &move = moves[i];
	BoardState savedState(board);
	board.make_move(move);
	/*
	if (board.playerInCheck(board.turn)) {
	    savedState.reapply(board);
	    continue;
	}*/
	int score;
	if (i == 0) score = -Negamax(board, -beta, -alpha, depth-1, searchDepth+1, pv, limit);
	else {
	    score = -Negamax(board, -alpha-1, -alpha, depth-2, searchDepth+1, pv, limit);
	    if (score > alpha && score < beta)
		score = -Negamax(board, -beta, -alpha, depth-1, searchDepth+1, pv, limit);
	}
	savedState.reapply(board);
	if (score >= beta) {
	    TT::StoreEval(hash, depth, searchDepth, beta, TT::probe_lower, move);
	    // check capture
	    if (!move.get_capture()) killer_moves[searchDepth] = move;
	    return beta;
	}
	if (score > alpha) {
	    ttBound = TT::probe_exact;
	    alpha = score;
	    bm = move;
	}
    }

    TT::StoreEval(hash, depth, searchDepth, alpha, ttBound, bm);
    return alpha;
}
} // namespace Functions

namespace {
PV ExtractPV(Board board) {
    size_t ply = board.ply;
    std::vector<BitMove> moves;
    while (moves.size() < 8) {
	BitMove move = TT::ProbeMove(board.get_hash());
	if (move.is_defined()) {
	    moves.push_back(move);
	    // board.make_move(move);
	}
	else
	    break;
    }

    return PV(ply, moves);
}

BitMove IterativeDeepening(Board &board, int timeLimit) {
    std::jmp_buf exitBuffer;
    SearchLimit limit = SearchLimit(exitBuffer, timeLimit);

    PV pv = PV();
    int alpha = -99999;
    int beta = 99999;
    
    for (size_t depth = 1; depth < 512 && !setjmp(exitBuffer); depth++) {
	auto t0 = std::chrono::steady_clock::now();
	// std::cout << "test" << std::endl;
	int score = Functions::Negamax(board, alpha, beta, depth, 0, pv, &limit);
	// std::cout << "found ";
	// std::cout << score << std::endl;
	if ((score <= alpha) || (score >= beta)) {
	    alpha = -99999;
	    beta = 99999;
	    score = Functions::Negamax(board, alpha, beta, depth, 0, pv, &limit);
	}
	else {
	    alpha = score - 50;
	    beta = score + 50;
	}
	auto t1 = std::chrono::steady_clock::now();
	size_t t = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
	timeLimit -= t;

	pv = ExtractPV(board);
	
	std::cout << "depth: " << depth << " score: " << score << " time: " << t << std::endl;
	
	/* for (size_t i = 0; i < pv.size(); i++)
	    std::cout << pv[i].Export() << " "; */
	std::cout << std::endl;
	std::flush(std::cout);
	if (std::abs(score) == 99999) break;
    }

    return pv[0];
}
} // namespace


BitMove BestMoveTime(Board &board, int timeLimit) {
    return IterativeDeepening(board, timeLimit);
}
} // namespace Search
