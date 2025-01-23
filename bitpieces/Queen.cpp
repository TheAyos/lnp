#include "Queen.h"

#include "../BitOps.h"
#include "Bishop.h"
#include "Rook.h"

using namespace BitOps;

namespace Queen {
    // FIXME: watch out for types, don't use int instead of U64 !!
    // OPTI https://www.chessprogramming.org/Magic_Bitboards
    U64 get_attack_masks_blocking(Board& board, int square, U64 blocking_square) {
        U64 attacks = 0ULL;

        // attacks = Bishop::get_attack_masks_blocking_slow(square, blocking_square);
        // attacks |= Rook::get_attack_masks_blocking_slow(square, blocking_square);
        attacks = Bishop::get_attack_masks_blocking_magic(board, square, blocking_square);
        attacks |= Rook::get_attack_masks_blocking_magic(board, square, blocking_square);

        return attacks;
    }

    void add_legal_moves(Board &board, MoveList &moves, bool onlyCaptures) {
        int turn = board.turn;
        U64 bb, attacks;

        int piece = (turn == W) ? QUEEN : queen;
        bb = board.bitboards[piece];

        while (bb) {
            int from = get_lsb_index(bb);

            // not capturing own pieces
            attacks = get_attack_masks_blocking(board, from, board.occupancies[WB]) & ~board.occupancies[turn];

            while (attacks) {
                int to = get_lsb_index(attacks);

                bool isCapture = get_bit(board.occupancies[1 - turn], to);
                if (!onlyCaptures || isCapture) {
	 	    // MoveList::Type T = isCapture ? MoveList::Quiet : MoveList::Attack; 
		    if (isCapture)
                    	board.add_move_if_legal<MoveList::Attack>(moves, BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));
		    else
			board.add_move_if_legal<MoveList::Quiet>(moves, BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));

                }
                clear_bit(attacks, to);
            }

            clear_bit(bb, from);
        }
    }
};  // namespace Rook
