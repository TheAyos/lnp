#include "Knight.h"

#include "../BitMove.h"
#include "../BitOps.h"
#include "../Board.h"

using namespace BitOps;

namespace Knight {
    U64 get_attack_mask(int square) {
        U64 attacks = 0ULL;
        U64 bb = 0ULL;
        set_bit(bb, square);

        if ((bb >> 17) & not_file_masks[h]) attacks |= (bb >> 17);  // NW
        if ((bb >> 15) & not_file_masks[a]) attacks |= (bb >> 15);  // NE
        if ((bb >> 10) & not_gh_file) attacks |= (bb >> 10);  // NWW
        if ((bb >> 6) & not_ab_file) attacks |= (bb >> 6);  // NEE

        if ((bb << 17) & not_file_masks[a]) attacks |= (bb << 17);  // SE
        if ((bb << 15) & not_file_masks[h]) attacks |= (bb << 15);  // SW
        if ((bb << 10) & not_ab_file) attacks |= (bb << 10);  // SEE
        if ((bb << 6) & not_gh_file) attacks |= (bb << 6);  // SWW

        return attacks;
    };

    void add_legal_moves(Board &board, BitMoveVec &moves) {
        int turn = board.turn;
        U64 bb, attacks;

        int piece = (turn == W) ? KNIGHT : knight;
        bb = board.bitboards[piece];

        while (bb) {
            int from = get_lsb_index(bb);

            // not capturing own pieces
            attacks = board.knight_attacks[from] & ~board.occupancies[turn];

            while (attacks) {
                int to = get_lsb_index(attacks);
                
                bool isCapture = get_bit(board.occupancies[1 - turn], to);
                moves.push_back(BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));

                clear_bit(attacks, to);
            }

            clear_bit(bb, from);
        }
    }
};  // namespace Knight