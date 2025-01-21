#include "Rook.h"

#include "../BitOps.h"

using namespace BitOps;

const int rookRelevancyIndexBits[64]
    = {12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10,
       10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10,
       10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

// resources:
// https://www.chessprogramming.org/Magic_Bitboards
// https://chess.stackexchange.com/questions/15070/understanding-magic-bitboard very helpful, helped debug

const U64 rookMagics[64] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,  0x100081001000420ULL,  0x200020010080420ULL,
    0x3001c0002010008ULL,  0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL,     0x2024401000200040ULL,
    0x100802000801000ULL,  0x120800800801000ULL,  0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL,
    0x801000060821100ULL,  0x80044006422000ULL,   0x100808020004000ULL,  0x12108a0010204200ULL, 0x140848010000802ULL,
    0x481828014002800ULL,  0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,      0x100400080208000ULL,
    0x2040002120081000ULL, 0x21200680100081ULL,   0x20100080080080ULL,   0x2000a00200410ULL,    0x20080800400ULL,
    0x80088400100102ULL,   0x80004600042881ULL,   0x4040008040800020ULL, 0x440003000200801ULL,  0x4200011004500ULL,
    0x188020010100100ULL,  0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,  0x200046502000484ULL,
    0x480400080088020ULL,  0x1000422010034000ULL, 0x30200100110040ULL,   0x100021010009ULL,     0x2002080100110004ULL,
    0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL, 0x101002200408200ULL,  0x40802000401080ULL,
    0x4008142004410100ULL, 0x2060820c0120200ULL,  0x1001004080100ULL,    0x20c020080040080ULL,  0x2935610830022400ULL,
    0x44440041009200ULL,   0x280001040802101ULL,  0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,  0x1004081002402ULL};

namespace Rook {

    // FIXME: watch out for types, don't use int instead of U64 !!
    U64 get_attack_masks_blocking_slow(int square, U64 blocking_square) {
        U64 attacks = 0ULL;
        int rank, file, target_rank, target_file;
        square_to_rf(square, target_rank, target_file);

        // N
        for (rank = target_rank + 1; rank < 8; rank++) {
            attacks |= (1ULL << rf_to_square(rank, target_file));
            if (blocking_square & (1ULL << rf_to_square(rank, target_file))) break;
        }
        // E
        for (file = target_file + 1; file < 8; file++) {
            attacks |= (1ULL << rf_to_square(target_rank, file));
            if (blocking_square & (1ULL << rf_to_square(target_rank, file))) break;
        }
        // S
        for (rank = target_rank - 1; rank >= 0; rank--) {
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

    /* ------------------------------- for magics ------------------------------- */

    U64 get_sliding_mask(int sq) {
        U64 attacks = 0ULL;
        int targetR, targetF, curR, curF;
        square_to_rf(sq, curR, curF);

        for (targetR = curR + 1; targetR <= 6; targetR++)
            attacks |= (1ULL << (targetR * 8 + curF));
        for (targetR = curR - 1; targetR >= 1; targetR--)
            attacks |= (1ULL << (targetR * 8 + curF));
        for (targetF = curF + 1; targetF <= 6; targetF++)
            attacks |= (1ULL << (curR * 8 + targetF));
        for (targetF = curF - 1; targetF >= 1; targetF--)
            attacks |= (1ULL << (curR * 8 + targetF));

        return attacks;
    }

    void initMagics(Board &board) {
        for (int sq = 0; sq < 64; sq++) {
            int nRelevantBits = BitOps::pop_count_bits(board.rookSlidingMasks[sq]);

            for (int bi = 0; bi < (1 << rookRelevancyIndexBits[sq]); bi++) {
                // U64 blockers = get_blockers(bi, board.rookSlidingMasks[sq]); // blockers v1
                U64 blockers = get_sliding_blockers(bi, nRelevantBits, board.rookSlidingMasks[sq]);
                // std::cout << get_attack_masks_blocking_slow(sq, blockers) << std::endl;
                board.rookMagicAttacks[sq][(blockers * rookMagics[sq]) >> (64 - rookRelevancyIndexBits[sq])]
                    = get_attack_masks_blocking_slow(sq, blockers);
            }
        }
    }

    U64 get_attack_masks_blocking_magic(Board &board, int square, U64 blocking_square) {
        blocking_square &= board.rookSlidingMasks[square];
        U64 magicIndexKey = (blocking_square * rookMagics[square]) >> (64 - rookRelevancyIndexBits[square]);
        return board.rookMagicAttacks[square][magicIndexKey];
    }

    void add_legal_moves(Board &board, BitMoveVec &moves, bool onlyCaptures) {
        int turn = board.turn;
        U64 bb, attacks;

        int piece = (turn == W) ? ROOK : rook;
        bb = board.bitboards[piece];

        while (bb) {
            int from = get_lsb_index(bb);

            // not capturing own pieces
            // attacks = get_attack_masks_blocking_slow(from, board.occupancies[WB]) & ~board.occupancies[turn];
            attacks = get_attack_masks_blocking_magic(board, from, board.occupancies[WB]) & ~board.occupancies[turn];

            while (attacks) {
                int to = get_lsb_index(attacks);

                bool isCapture = get_bit(board.occupancies[1 - turn], to);
                if (!onlyCaptures || isCapture) {
                    board.add_move_if_legal(
                        moves, BitMove(from, to, piece, NO_PROMOTION, isCapture, false, false, false));
                }
                clear_bit(attacks, to);
            }

            clear_bit(bb, from);
        }
    }
};  // namespace Rook