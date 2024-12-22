#include <iostream>
#include <time.h>
#include "BitBoard.h"
#include "BitMove.h"
#include "bitpieces/Pawn.h"

/* -------------------------------------------------------------------------- */
/*                        pieces attack precalculation                        */
/* -------------------------------------------------------------------------- */
U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];

/* -------------------------------------------------------------------------- */
/*                       leaper attack masks generation                       */
/* -------------------------------------------------------------------------- */
U64 get_knight_attack_mask(int square) {
    U64 attacks = 0ULL;
    U64 bb = 0ULL;
    set_bit(bb, square);

    if ((bb >> 17) & not_file_masks[h]) attacks |= (bb >> 17); // NW
    if ((bb >> 15) & not_file_masks[a]) attacks |= (bb >> 15); // NE
    if ((bb >> 10) & not_gh_file) attacks |= (bb >> 10); // NWW
    if ((bb >> 6) & not_ab_file) attacks |= (bb >> 6); // NEE

    if ((bb << 17) & not_file_masks[a]) attacks |= (bb << 17); // SE
    if ((bb << 15) & not_file_masks[h]) attacks |= (bb << 15); // SW
    if ((bb << 10) & not_ab_file) attacks |= (bb << 10); // SEE
    if ((bb << 6) & not_gh_file) attacks |= (bb << 6); // SWW

    return attacks;
}
U64 get_king_attack_mask(int square) {
    U64 attacks = 0ULL;
    U64 bb = 0ULL;
    set_bit(bb, square);

    if (bb >> 8) attacks |= (bb >> 8); // N // (bb >> 8) sufficient to check if on board here
    if (bb << 8) attacks |= (bb << 8); // S
    if ((bb >> 1) & not_file_masks[h]) attacks |= (bb >> 1); // W
    if ((bb << 1) & not_file_masks[a]) attacks |= (bb << 1); // E
    if ((bb >> 7) & not_file_masks[a]) attacks |= (bb >> 7); // NE
    if ((bb << 7) & not_file_masks[h]) attacks |= (bb << 7); // SW
    if ((bb >> 9) & not_file_masks[h]) attacks |= (bb >> 9); // NW
    if ((bb << 9) & not_file_masks[a]) attacks |= (bb << 9); // SE

    return attacks;
}
U64 get_bishop_attack_masks(int square) {
    // https://www.chessprogramming.org/Magic_Bitboards
    U64 attacks = 0ULL;
    int rank, file, target_rank, target_file;
    square_to_rf(square, target_rank, target_file);

    // generate diagonal occupancy masks for bishop
    // NE
    for (rank = target_rank - 1, file = target_file + 1; rank > 0 && file < 7; rank--, file++)
        attacks |= (1ULL << rf_to_square(rank, file));
    // SE
    for (rank = target_rank + 1, file = target_file + 1; rank < 7 && file < 7; rank++, file++)
        attacks |= (1ULL << rf_to_square(rank, file));
    // SW
    for (rank = target_rank + 1, file = target_file - 1; rank < 7 && file > 0; rank++, file--)
        attacks |= (1ULL << rf_to_square(rank, file));
    // NW
    for (rank = target_rank - 1, file = target_file - 1; rank > 0 && file > 0; rank--, file--)
        attacks |= (1ULL << rf_to_square(rank, file));

    return attacks;
}
U64 get_rook_attack_masks(int square) {
    // https://www.chessprogramming.org/Magic_Bitboards
    U64 attacks = 0ULL;
    int rank, file, target_rank, target_file;
    square_to_rf(square, target_rank, target_file);

    // generate vertical/horizontal occupancy masks for rook
    // N
    for (rank = target_rank - 1; rank > 0; rank--)
        attacks |= (1ULL << rf_to_square(rank, target_file));
    // E
    for (file = target_file + 1; file < 7; file++)
        attacks |= (1ULL << rf_to_square(target_rank, file));
    // S
    for (rank = target_rank + 1; rank < 7; rank++)
        attacks |= (1ULL << rf_to_square(rank, target_file));
    // W
    for (file = target_file - 1; file > 0; file--)
        attacks |= (1ULL << rf_to_square(target_rank, file));

    return attacks;
}

//FIXME: watch out for types, don't use int instead of U64 !!
U64 get_bishop_attack_masks_blocking(int square, U64 blocking_square) {
    // https://www.chessprogramming.org/Magic_Bitboards
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
U64 get_rook_attack_masks_blocking(int square, U64 blocking_square) {
    // https://www.chessprogramming.org/Magic_Bitboards
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
// go over all possible occupancies for given attack_mask (for sliding piece)
U64 set_occupancy(int index, int mask_nbits, U64 attack_mask) {
    U64 occupancy = 0ULL;

    for (int n = 0; n < mask_nbits; n++) {
        int square = get_lsb_index(attack_mask);
        clear_bit(attack_mask, square);
        // check that occupancy is in bounds
        if (index & (1 << n)) occupancy |= (1ULL << square);
    }

    return occupancy;
}
/* -------------------------------------------------------------------------- */
/*                                   magic                                    */
/* -------------------------------------------------------------------------- */
const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};
const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};
// rook magic numbers
U64 rook_magic_numbers[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL
};
// bishop magic numbers
U64 bishop_magic_numbers[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL
};
U64 bishop_masks[64];
U64 rook_masks[64];
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];
void init_slider_attacks(int bishop) {
    for (int square = 0; square < 64; square++) {
        bishop_masks[square] = get_bishop_attack_masks(square);
        rook_masks[square] = get_rook_attack_masks(square);

        U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];
        
        int relevant_bits = count_bits(attack_mask);
        int occupancy_indices = 1 << relevant_bits;

        for (int i = 0; i < occupancy_indices; i++) {
            if (bishop) {
                U64 occupancy = set_occupancy(i, relevant_bits, attack_mask);
                int magic_index = (int)((occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]));
                bishop_attacks[square][magic_index] = get_bishop_attack_masks_blocking(square, occupancy);
            } else {
                U64 occupancy = set_occupancy(i, relevant_bits, attack_mask);
                int magic_index = (int)((occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]));
                rook_attacks[square][magic_index] = get_rook_attack_masks_blocking(square, occupancy);
            }
        }
    }
}
static inline U64 get_bishop_attacks(int square, U64 occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= (64 - bishop_relevant_bits[square]);
    return bishop_attacks[square][occupancy];
}
static inline U64 get_rook_attacks(int square, U64 occupancy) {
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= (64 - rook_relevant_bits[square]);
    return rook_attacks[square][occupancy];
}
U64 find_magic_number(int square, int relevant_bits, int bishop) {
    // https://www.chessprogramming.org/Looking_for_Magics
    std::srand(time(nullptr));
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];
    U64 attack_mask = bishop ? get_bishop_attack_masks(square) : get_rook_attack_masks(square);
    int occupancy_indices = 1 << relevant_bits;

    for (int i = 0; i < occupancy_indices; i++) {
        occupancies[i] = set_occupancy(i, relevant_bits, attack_mask);
        attacks[i] = bishop ? get_bishop_attack_masks_blocking(square, occupancies[i]) : 
                              get_rook_attack_masks_blocking(square, occupancies[i]);
    }

    for (int attempt = 0; attempt < 100000000; attempt++) {
        U64 magic_number_candidate = ((U64)std::rand() & 0xFFFF) << 48 | ((U64)std::rand() & 0xFFFF) << 32 | ((U64)std::rand() & 0xFFFF) << 16 | ((U64)std::rand() & 0xFFFF);
        if (count_bits((attack_mask * magic_number_candidate) & 0xFF00000000000000) < 6) continue;
        std::fill(std::begin(used_attacks), std::end(used_attacks), 0ULL);
        bool fail = false;

        for (int i = 0; i < occupancy_indices && !fail; i++) {
            int magic_index = (int)((occupancies[i] * magic_number_candidate) >> (64 - relevant_bits));

            if (used_attacks[magic_index] == 0ULL) {
                used_attacks[magic_index] = attacks[i];
            } else if (used_attacks[magic_index] != attacks[i]) {
                fail = true;
            }
        }
        if (!fail) return magic_number_candidate;
    }
    std::cout << "Failed to find a magic number" << std::endl;
    return 0ULL;
}
void init_magic_numbers() {
    for (int square = 0; square < 64; square++) {
        bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], true);
        rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], false);
    }
}


int main(int argc, char* argv[]) {
    
    #if 0
    /* -------------------------------------------------------------------------- */
    /*                                    enums                                   */
    /* -------------------------------------------------------------------------- */

    /* --------------------------------- squares -------------------------------- */
    for (int rank = 8; rank >= 1; rank--){
        for (int file = 'a'; file < 'a'+8; file++){
            std::cout << (char)file << rank << ',';
        }
        std::cout << std::endl;
    }

    /* -------------------------------------------------------------------------- */
    /*                                    masks                                   */
    /* -------------------------------------------------------------------------- */
    /* ------------------------- generate not file masks ------------------------ */
    U64 not_file_gen[8] = {0ULL};
    std::cout << "not_file_gen = {" << std::endl;
    for (int tfile = a; tfile <= h; tfile++) {
        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++){
                if (file == tfile) continue;
                set_bit(not_file_gen[tfile], rf_to_square(rank, file));
            }
        }
        std::cout << not_file_gen[tfile] << "ULL," << std::endl;
    }
    std::cout << "};" << std::endl;
    for (int tfile = a; tfile < h; tfile++) display(not_file_gen[tfile]);
    display(not_gh_file);
    display(not_ab_file);




    /* --------------------------------- attacks -------------------------------- */
    for (int square = 0; square < 64; square++) {
        display(pawn_attacks[W][square]);
        display(pawn_attacks[B][square]);
    }
    for (int square = 0; square < 64; square++) {
        display(knight_attacks[square]);
    }
    for (int square = 0; square < 64; square++) {
        display(king_attacks[square]);
    }
    for (int square = 0; square < 64; square++) {
        display_bits(get_bishop_attack_masks(square));
    }
    for (int square = 0; square < 64; square++) {
        display_bits(get_rook_attack_masks(square));
    }
    // U64 blocking_square = 0ULL;
    // set_bit(blocking_square, e3);
    // set_bit(blocking_square, b2);
    // set_bit(blocking_square, g7);
    // set_bit(blocking_square, b6);
    // display_bits(blocking_square);

    // display_bits(get_bishop_attack_masks_blocking(d4, blocking_square));

    // blocking_square = 0ULL;
    // set_bit(blocking_square, d7);
    // set_bit(blocking_square, d2);
    // set_bit(blocking_square, g4);
    // set_bit(blocking_square, b4);
    // display_bits(blocking_square);
    // display_bits(get_rook_attack_masks_blocking(d4, blocking_square));
    U64 bb = 0ULL;
    set_bit(bb, e2);
    set_bit(bb, c2);
    set_bit(bb, h4);
    display_bits(bb);
    std::cout << count_bits(bb) << std::endl;
    std::cout << get_lsb_index(bb) << std::endl;
    clear_bit(bb, get_lsb_index(bb));
    std::cout << get_lsb_index(bb) << std::endl;
    std::cout << count_bits(bb) << std::endl;
    display_bits(bb);
    U64 attack_mask = get_rook_attack_masks(a1);

    for (int i = 0; i < 4096; i++){
        U64 occ = set_occupancy(i, count_bits(attack_mask), attack_mask);
        display_bits(occ);
        // getchar();
    }
    attack_mask = get_bishop_attack_masks(e2);

    for (int i = 0; i < 4096; i++){
        U64 occ = set_occupancy(i, count_bits(attack_mask), attack_mask);
        display_bits(occ);
        // getchar();
    }
    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            int sq = r * 8 + f;
            std::cout << count_bits(get_bishop_attack_masks(sq)) << ", ";
        }
        std::cout << std::endl;
    }
    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            int sq = r * 8 + f;
            std::cout << count_bits(get_rook_attack_masks(sq)) << ", ";
        }
        std::cout << std::endl;
    }
    init_slider_attacks(true);
    init_slider_attacks(false);
    
    U64 occ = 0ULL;
    set_bit(occ, c5);
    set_bit(occ, e4);
    set_bit(occ, e2);
    display_bits(occ);
    display_bits(get_bishop_attacks(d4, occ));
    
    display_bits(get_rook_attacks(d4, occ));
    #endif

    // precalculate attacks
    for (int square = 0; square < 64; square++) {
        pawn_attacks[W][square] = Pawn::get_attack_mask(W, square);
        pawn_attacks[B][square] = Pawn::get_attack_mask(B, square);

        // knight_attacks[square] = get_knight_attack_mask(square);
        // king_attacks[square] = get_king_attack_mask(square);
    }
    // init_magic_numbers();

    // Board board;
    // display_bits(board.bitboards[PAWN]);
    // std::cout << board << std::endl;

    // printf("%c\n", letter_pieces[char_to_pieces['Q']]);

    // BitMove move(g2, d8, queen, ROOK, true, 0, 0, PAWN);
    // move.print();

    // std::cout << move.get_algebraic_notation() << std::endl;

    return 0;
}