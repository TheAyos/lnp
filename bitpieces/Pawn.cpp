#include "Pawn.h"

#include "../BitMove.h"
#include "../BitOps.h"
#include "../Board.h"

using namespace BitOps;

namespace Pawn {
    U64 get_attack_mask(int color, int square) {
        U64 attacks = 0ULL;  // attacks bitmask
        U64 bb = 0ULL;  // bitboard
        set_bit(bb, square);  // bit position

        if (color == W) {
            if ((bb >> 7) & not_file_masks[a])
                attacks |= (bb >> 7);  // capture to the right front & make sure not to wrap around edge of board
            if ((bb >> 9) & not_file_masks[h])
                attacks |= (bb >> 9);  // capture to the left front & make sure not to wrap around edge of board
        } else {
            if ((bb << 7) & not_file_masks[h])
                attacks |= (bb << 7);  // ...
            if ((bb << 9) & not_file_masks[a])
                attacks |= (bb << 9);
        }

        return attacks;
    };

    void add_legal_moves(Board &board, BitMoveVec &moves) {
        int turn = board.turn;
        U64 bb, attacks;  // TODO: replace while by iterator on BB

        int piece = (turn == W) ? PAWN : pawn;

        bb = board.bitboards[piece];

        // forall pawns
        while (bb) {
            // for (auto it = BitIterator::begin(board.bitboards[piece]); it != BitIterator::end(); ++it) {
            int from, to;
            from = get_lsb_index(bb);
            // from = *it;
            // pawn attacks at least one square ahead
            to = (turn == W) ? from - 8 : from + 8;

            // if not on first/last rank (i.e. if pawn can move forward)
            if (((turn == W && !(to < a8)) || (turn == B && !(to > h1))) && !get_bit(board.occupancies[WB], to)) {
                // pawn promotion placement
                if ((turn == W && from >= a7 && from <= h7) || (turn == B && from >= a2 && from <= h2)) {
                    moves.push_back(BitMove(
                        from, to, piece, QUEEN + 6 * turn, false, false, false,
                        false));  // QUEEN is white, if turn is black (turn = 1), then QUEEN + 6*1 is queen (black)
                    moves.push_back(BitMove(from, to, piece, ROOK + 6 * turn, false, false, false, false));  //...
                    moves.push_back(BitMove(from, to, piece, BISHOP + 6 * turn, false, false, false, false));
                    moves.push_back(BitMove(from, to, piece, KNIGHT + 6 * turn, false, false, false, false));
                } else {  // "normal" pawn moves
                    moves.push_back(BitMove(from, to, piece, NO_PROMOTION, false, false, false, false));
                    // double pawn push position and two squares ahead is free
                    if (((turn == W && from >= a2 && from <= h2) || turn == B && from >= a7 && from <= h7)
                        && !get_bit(board.occupancies[WB], turn == W ? to - 8 : to + 8)) {
                        moves.push_back(
                            BitMove(from, turn == W ? to - 8 : to + 8, piece, NO_PROMOTION, false, true, false, false));
                    }
                }
            }

            // keep attacks that land on enemy pieces
            attacks = board.pawn_attacks[turn][from] & board.occupancies[1 - turn];
            while (attacks) {
                // for (auto it = BitOps::iterator(attacks); it != BitOps::iterator(); ++it) {
                // to = *it;

                to = get_lsb_index(attacks);
                // pawn promotion placement and not empty target (i.e. capture)
                if ((turn == W && from >= a7 && from <= h7) || (turn == B && from >= a2 && from <= h2)) {
                    moves.push_back(BitMove(from, to, piece, QUEEN + 6 * turn, true, false, false, false));
                    moves.push_back(BitMove(from, to, piece, ROOK + 6 * turn, true, false, false, false));
                    moves.push_back(BitMove(from, to, piece, BISHOP + 6 * turn, true, false, false, false));
                    moves.push_back(BitMove(from, to, piece, KNIGHT + 6 * turn, true, false, false, false));
                } else {
                    moves.push_back(BitMove(from, to, piece, NO_PROMOTION, true, false, false, false));
                }
                clear_bit(attacks, to);
            }

            if (board.enpassant_square != -1) {
                U64 enpassant_attacks = board.pawn_attacks[turn][from] & (1ULL << board.enpassant_square);

                if (enpassant_attacks) {
                    int enpassant_to = get_lsb_index(enpassant_attacks);
                    moves.push_back(BitMove(from, enpassant_to, piece, NO_PROMOTION, true, false, true, false));
                }
            }

            clear_bit(bb, from);
        }
    }
};  // namespace Pawn