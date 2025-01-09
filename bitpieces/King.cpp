#include "King.h"

#include "../BitOps.h"

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
            attacks = board.kingAttacks[from] & ~board.occupancies[turn];

            while (attacks) {
                int to = get_lsb_index(attacks);

                bool isCapture = get_bit(board.occupancies[1 - turn], to);
                // FIXME: don't add if move puts king in check here ?
                // moves.push_back(BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));
                // add only if not attacked by enemy
                // moves.add_move_if_legal(
                //     BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false), board);

                board.add_move_if_legal(moves, BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));


                clear_bit(attacks, to);
            }

            clear_bit(bb, from);
        }

        /* castling rights bit template (takes 4 bits)
            - 0001 : white kingside
            - 0010 : white queenside
            - 0100 : black kingside
            - 1000 : black queenside
            ...
            - 0000 : no castling
            - 1111 : all castling rights
        */
        // enum CastlingRights { WK = 1, WQ = 2, BK = 4, BQ = 8 };

        // https://en.wikipedia.org/wiki/Castling#Rules
        // Castling is permitted provided all of the following conditions are met:[5]
        // Neither the king nor the rook has previously moved. (1)
        // There are no pieces between the king and the rook. (2)
        // The king is not currently in check. (3)
        // The king does not pass through or finish on a square that is attacked by an enemy piece. (4)

        // generate castling moves according to current turn
        int kingPiece = (turn == W) ? KING : king;
        int kingSideRookSquare = (turn == W) ? g1 : g8;
        int queenSideRookSquare = (turn == W) ? c1 : c8;
        int kingSideEmptySquares[] = {(turn == W) ? f1 : f8, (turn == W) ? g1 : g8};
        int queenSideEmptySquares[] = {(turn == W) ? c1 : c8, (turn == W) ? d1 : d8, (turn == W) ? b1 : b8};
        int kingSideCastlingRight = (turn == W) ? WK : BK;
        int queenSideCastlingRight = (turn == W) ? WQ : BQ;
        int kingInitialSquare = (turn == W) ? e1 : e8;

        // check (1)
        if (board.castlingRights & kingSideCastlingRight) {
            // check (2)
            bool empty = true;
            for (int square : kingSideEmptySquares) {
                if (!board.is_empty(square)) {
                    empty = false;
                    break;
                }
            }
            if (empty) {
                // check (3) and (4)
                //FIXME: add check is attacked on other kingsideemptysquare ?
                if (!board.is_attacked(kingInitialSquare, 1 - turn) && !board.is_attacked(kingSideEmptySquares[0], 1 - turn))
                board.add_move_if_legal(moves, BitMove(kingInitialSquare, kingSideRookSquare, piece, NO_PROMOTION, false, false, false, true));
                // moves.push_back();
            }
        }
        if (board.castlingRights & queenSideCastlingRight) {
            bool empty = true;
            for (int square : queenSideEmptySquares) {
                if (!board.is_empty(square)) {
                    empty = false;
                    break;
                }
            }
            if (empty) {
                //FIXME: add check is attacked on other queensideemptysquare ?
                if (!board.is_attacked(kingInitialSquare, 1 - turn) && !board.is_attacked(queenSideEmptySquares[1], 1 - turn))
                    board.add_move_if_legal(moves, BitMove(kingInitialSquare, queenSideRookSquare, piece, NO_PROMOTION, false, false, false, true));
                    // moves.push_back(BitMove(kingInitialSquare, queenSideRookSquare, piece, NO_PROMOTION, false, false, false, true));
            }
        }
    }
};  // namespace King