#include <iostream>
#include "BitBoard.h"

const U64 not_file[8] = {
    18374403900871474942ULL,
    18302063728033398269ULL,
    18157383382357244923ULL,
    17868022691004938231ULL,
    17289301308300324847ULL,
    16131858542891098079ULL,
    13816973012072644543ULL,
    9187201950435737471ULL,
};

const U64 not_gh_file = not_file[h] & not_file[g];
const U64 not_ab_file = not_file[a] & not_file[b];



U64 get_pawn_attack_masks(int color, int square) {
    U64 attacks = 0ULL;

    // bitboard
    U64 bb = 0ULL;
    set_bit(bb, square);

    if (color == white) {
        if ((bb >> 7) & not_file[a]) attacks |= (bb >> 7); // capture to the right front
        if ((bb >> 9) & not_file[h]) attacks |= (bb >> 9); // capture to the left front
    } else {
        if ((bb << 7) & not_file[h]) attacks |= (bb << 7); // ...
        if ((bb << 9) & not_file[a]) attacks |= (bb << 9);
    }

    return attacks;
}

/* -------------------------------------------------------------------------- */
/*                       leaper attack masks generation                       */
/* -------------------------------------------------------------------------- */
U64 pawn_attacks[2][64];
void generate_leaper_attack_masks() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[white][square] = get_pawn_attack_masks(white, square);
        pawn_attacks[black][square] = get_pawn_attack_masks(black, square);
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
    // for (int tfile = a; tfile < h; tfile++) display(not_file_gen[tfile]);
    // display(not_gh_file);
    // display(not_ab_file);
    #endif

    /* --------------------------------- attacks -------------------------------- */
    generate_leaper_attack_masks();

    // for (int square = 0; square < 64; square++) {
    //     display(pawn_attacks[white][square]);
    //     display(pawn_attacks[black][square]);
    // }

    display(pawn_attacks[white][19]);
    display_bits(pawn_attacks[white][19]);

    return 0;
}