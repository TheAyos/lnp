#pragma once
#include "BitMove.h"
#include "Definitions.h"

// TODO: replace with references where possible
class Board {
   public:
    U64 bitboards[12];
    U64 occupancies[3];
    int turn;
    int enpassantSquare;
    int castlingRights;

    Board(bool initpieces = true);
    Board(const std::string &fen);
    void clear_board();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
    int get_piece_on_square(int square);
    bool is_empty(int square);

    BitMove parse_algebraic_move(int from, int to, char promotion_piece);
    void move(BitMove &move);

    // precalculated attack bitmasks
    U64 pawnAttacks[2][64] = {0};
    U64 knightAttacks[64] = {0};
    U64 kingAttacks[64] = {0};

    bool is_attacked(int square, int by_turn);

    BitMoveVec get_all_legal_moves();

   private:
    void setup_initial_pieces();
    void update_occupancies();

    void init_attacks();
};