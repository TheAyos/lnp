#include "Rook.h"

#include "../BitMove.h"
#include "../BitOps.h"
#include "../Board.h"

using namespace BitOps;

namespace Rook {
    // FIXME: watch out for types, don't use int instead of U64 !!
    // OPTI https://www.chessprogramming.org/Magic_Bitboards
    U64 get_attack_masks_blocking(int square, U64 blocking_square) {
        U64 attacks = 0ULL;
        int rank, file, target_rank, target_file;
        square_to_rf(square, target_rank, target_file);

        // N
        for (rank = target_rank - 1; rank >= 0; rank--) {
            attacks |= (1ULL << rf_to_square(rank, target_file));
            if (blocking_square & (1ULL << rf_to_square(rank, target_file))) break;
        }
        // E
        for (file = target_file + 1; file < 8; file++) {
            attacks |= (1ULL << rf_to_square(target_rank, file));
            if (blocking_square & (1ULL << rf_to_square(target_rank, file))) break;
        }
        // S
        for (rank = target_rank + 1; rank < 8; rank++) {
            attacks |= (1ULL << rf_to_square(rank, target_file));
            if (blocking_square & (1ULL << rf_to_square(rank, target_file))) break;
        }
        // W
        for (file = target_file - 1; file >= 0; file--) {
            attacks |= (1ULL << rf_to_square(target_rank, file));
            if (blocking_square & (1ULL << rf_to_square(target_rank, file))) break;
        }

        return attacks;
    }

    void add_legal_moves(Board &board, BitMoveVec &moves) {
        int turn = board.turn;
        U64 bb, attacks;

        int piece = (turn == W) ? ROOK : rook;
        bb = board.bitboards[piece];

        while (bb) {
            int from = get_lsb_index(bb);

            // not capturing own pieces
            attacks = get_attack_masks_blocking(from, board.occupancies[WB]) & ~board.occupancies[turn];

            while (attacks) {
                int to = get_lsb_index(attacks);

                bool isCapture = get_bit(board.occupancies[1 - turn], to);
                moves.push_back(BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));

                clear_bit(attacks, to);
            }

            clear_bit(bb, from);
        }
    }
};  // namespace Rook