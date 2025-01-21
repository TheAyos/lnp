#include "Bishop.h"

#include "../BitOps.h"

using namespace BitOps;

const int bishopRelevancyIndexBits[64]
    = {6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5,
       5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};

const U64 bishopMagics[64] = {
    0x40040844404084ULL,   0x2004208a004208ULL,   0x10190041080202ULL,   0x108060845042010ULL,  0x581104180800210ULL,
    0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL,  0x4050404440404ULL,    0x21001420088ULL,
    0x24d0080801082102ULL, 0x1020a0a020400ULL,    0x40308200402ULL,      0x4011002100800ULL,    0x401484104104005ULL,
    0x801010402020200ULL,  0x400210c3880100ULL,   0x404022024108200ULL,  0x810018200204102ULL,  0x4002801a02003ULL,
    0x85040820080400ULL,   0x810102c808880400ULL, 0xe900410884800ULL,    0x8002020480840102ULL, 0x220200865090201ULL,
    0x2010100a02021202ULL, 0x152048408022401ULL,  0x20080002081110ULL,   0x4001001021004000ULL, 0x800040400a011002ULL,
    0xe4004081011002ULL,   0x1c004001012080ULL,   0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL,
    0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL,
    0x209188240001000ULL,  0x400408a884001800ULL, 0x110400a6080400ULL,   0x1840060a44020800ULL, 0x90080104000041ULL,
    0x201011000808101ULL,  0x1a2208080504f080ULL, 0x8012020600211212ULL, 0x500861011240000ULL,  0x180806108200800ULL,
    0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL,  0x20906061210001ULL,   0x5a84841004010310ULL,
    0x4010801011c04ULL,    0xa010109502200ULL,    0x4a02012000ULL,       0x500201010098b028ULL, 0x8040002811040900ULL,
    0x28000010020204ULL,   0x6000020202d0240ULL,  0x8918844842082200ULL, 0x4010011029020020ULL};

// resources:
// https://www.chessprogramming.org/Magic_Bitboards

namespace Bishop {

    // FIXME: watch out for types, don't use int instead of U64 !!
    U64 get_attack_masks_blocking_slow(int square, U64 blocking_square) {
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

    /* ------------------------------- for magics ------------------------------- */

    U64 get_sliding_mask(int sq) {
        U64 attacks = 0ULL;
        int targetR, targetF, curR, curF;
        square_to_rf(sq, curR, curF);

        for (targetR = curR + 1, targetF = curF + 1; targetR <= 6 && targetF <= 6; targetR++, targetF++) attacks |= (1ULL << (targetR * 8 + targetF));
        for (targetR = curR - 1, targetF = curF + 1; targetR >= 1 && targetF <= 6; targetR--, targetF++) attacks |= (1ULL << (targetR * 8 + targetF));
        for (targetR = curR + 1, targetF = curF - 1; targetR <= 6 && targetF >= 1; targetR++, targetF--) attacks |= (1ULL << (targetR * 8 + targetF));
        for (targetR = curR - 1, targetF = curF - 1; targetR >= 1 && targetF >= 1; targetR--, targetF--) attacks |= (1ULL << (targetR * 8 + targetF));
        
        return attacks;
    }

    void initMagics(Board &board) {
        for (int sq = 0; sq < 64; sq++) {
            int nRelevantBits = BitOps::pop_count_bits(board.bishopSlidingMasks[sq]);

            for (int bi = 0; bi < (1 << bishopRelevancyIndexBits[sq]); bi++) {
                U64 blockers = get_sliding_blockers(bi, nRelevantBits, board.bishopSlidingMasks[sq]);
                board.bishopMagicAttacks[sq][(blockers * bishopMagics[sq]) >> (64 - bishopRelevancyIndexBits[sq])]
                    = get_attack_masks_blocking_slow(sq, blockers);
            }
        }
    }

    U64 get_attack_masks_blocking_magic(Board &board, int square, U64 blocking_square) {
        blocking_square &= board.bishopSlidingMasks[square];
        U64 magicIndexKey = (blocking_square * bishopMagics[square]) >> (64 - bishopRelevancyIndexBits[square]);
        return board.bishopMagicAttacks[square][magicIndexKey];
    }

    void add_legal_moves(Board &board, BitMoveVec &moves, bool onlyCaptures) {
        int turn = board.turn;
        U64 bb, attacks;

        int piece = (turn == W) ? BISHOP : bishop;
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
};  // namespace Bishop