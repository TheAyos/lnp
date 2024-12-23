#pragma once
#include "BitMove.h"
#include "Definitions.h"

// TODO: replace with references where possible
class Board {
   public:
    U64 bitboards[12];
    U64 occupancies[3];
    int turn;
    int enpassant_square;
    int castling_rights;

    Board(bool initpieces = true);
    Board(const std::string &fen);
    void clear_board();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
    int get_piece_on_square(int square);
    BitMove parse_algebraic_move(int from, int to, char promotion_piece);
    void move(BitMove &move);

    BitMoveVec get_all_legal_moves();

    U64 pawn_attacks[2][64] = {0};
    U64 knight_attacks[64] = {0};
    U64 king_attacks[64] = {0};

   private:
    void setup_initial_pieces();
    void update_occupancies();

    void init_attacks();
};