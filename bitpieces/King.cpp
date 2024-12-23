#include "King.h"

#include "../BitMove.h"
#include "../BitOps.h"
#include "../Board.h"

using namespace BitOps;

namespace King {
    U64 get_attack_mask(int square) {
        U64 attacks = 0ULL;
        U64 bb = 0ULL;
        set_bit(bb, square);

        if (bb >> 8) attacks |= (bb >> 8);  // N // (bb >> 8) sufficient to check if on board here
        if (bb << 8) attacks |= (bb << 8);  // S
        if ((bb >> 1) & not_file_masks[h]) attacks |= (bb >> 1);  // W
        if ((bb << 1) & not_file_masks[a]) attacks |= (bb << 1);  // E
        if ((bb >> 7) & not_file_masks[a]) attacks |= (bb >> 7);  // NE
        if ((bb << 7) & not_file_masks[h]) attacks |= (bb << 7);  // SW
        if ((bb >> 9) & not_file_masks[h]) attacks |= (bb >> 9);  // NW
        if ((bb << 9) & not_file_masks[a]) attacks |= (bb << 9);  // SE

        return attacks;
    };

    void add_legal_moves(Board &board, BitMoveVec &moves) {
        int turn = board.turn;
        U64 bb, attacks;

        int piece = (turn == W) ? KING : king;
        bb = board.bitboards[piece];

        while (bb) {
            int from = get_lsb_index(bb);

            // not capturing own pieces
            attacks = board.king_attacks[from] & ~board.occupancies[turn];

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