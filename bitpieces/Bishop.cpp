#include "Bishop.h"

#include "../BitOps.h"

using namespace BitOps;

namespace Bishop {
    // FIXME: watch out for types, don't use int instead of U64 !!
    // OPTI https://www.chessprogramming.org/Magic_Bitboards
    U64 get_attack_masks_blocking(int square, U64 blocking_square) {
        U64 attacks = 0ULL;
        int rank, file, target_rank, target_file;
        square_to_rf(square, target_rank, target_file);

        // NE
        for (rank = target_rank - 1, file = target_file + 1; rank >= 0 && file < 8; rank--, file++) {
            attacks |= (1ULL << rf_to_square(rank, file));
            if (blocking_square & (1ULL << rf_to_square(rank, file))) break;
        }
        // SE
        for (rank = target_rank + 1, file = target_file + 1; rank < 8 && file < 8; rank++, file++) {
            attacks |= (1ULL << rf_to_square(rank, file));
            if (blocking_square & (1ULL << rf_to_square(rank, file))) break;
        }
        // SW
        for (rank = target_rank + 1, file = target_file - 1; rank < 8 && file >= 0; rank++, file--) {
            attacks |= (1ULL << rf_to_square(rank, file));
            if (blocking_square & (1ULL << rf_to_square(rank, file))) break;
        }
        // NW
        for (rank = target_rank - 1, file = target_file - 1; rank >= 0 && file >= 0; rank--, file--) {
            attacks |= (1ULL << rf_to_square(rank, file));
            if (blocking_square & (1ULL << rf_to_square(rank, file))) break;
        }

        return attacks;
    }

    void add_legal_moves(Board &board, BitMoveVec &moves, bool onlyCaptures) {
        int turn = board.turn;
        U64 bb, attacks;

        int piece = (turn == W) ? BISHOP : bishop;
        bb = board.bitboards[piece];

        while (bb) {
            int from = get_lsb_index(bb);

            // not capturing own pieces
            attacks = get_attack_masks_blocking(from, board.occupancies[WB]) & ~board.occupancies[turn];

            while (attacks) {
                int to = get_lsb_index(attacks);

                bool isCapture = get_bit(board.occupancies[1 - turn], to);
                if (!onlyCaptures || isCapture) {
                    board.add_move_if_legal(moves, BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));
                }

                clear_bit(attacks, to);
            }

            clear_bit(bb, from);
        }
    }
};  // namespace Bishop