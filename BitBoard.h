#pragma once
#include <iostream>

#include "BitOps.h"
#include "Definitions.h"
#include "BitMove.h"

class Board {
   public:
    U64 bitboards[12];
    U64 occupancies[3];

    int turn = W;
    int enpassant_square = -1;
    int castling_rights = 0;

    Board();

    // https://www.geeksforgeeks.org/friend-class-function-cpp/
    friend std::ostream& operator<<(std::ostream& os, const Board& board);

    void move(BitMove &move) {
        //TODO: save state
        int from = move.get_from();
        int to = move.get_to();
        int piece = move.get_piece();
        int promotion = move.get_promoted();
        int capture = move.get_capture();
        int doublepush = move.get_doublepush();
        int enpassant = move.get_enpassant();
        int castling = move.get_castling();

        // sanitize parser input
        // search for piece
        if(piece == NOPIECE) {
            for (int bb_piece = PAWN; bb_piece <= king; bb_piece++) {
                if (get_bit(bitboards[bb_piece], from)) {
                    piece = bb_piece;
                    break;
                }
            }
        }

        clear_bit(bitboards[piece], from); // remove piece from old square
        set_bit(bitboards[piece], to); // add piece to new square

        //TODO: handle captures
        //TODO: handle promotions
        //TODO: handle enpassant
        //TODO: handle castling
        //TODO: handle checks
    }

    void generate_moves();

   private:
    void setup_initial_pieces();
};

Board::Board() {
    setup_initial_pieces();
}

void Board::setup_initial_pieces() {
    set_bit(bitboards[PAWN], g2);
    set_bit(bitboards[PAWN], h2);
    set_bit(bitboards[PAWN], f2);
    set_bit(bitboards[PAWN], e2);
    set_bit(bitboards[PAWN], d2);
    set_bit(bitboards[PAWN], a2);
    set_bit(bitboards[PAWN], b2);
    set_bit(bitboards[PAWN], c2);

    set_bit(bitboards[pawn], g7);
    set_bit(bitboards[pawn], h7);
    set_bit(bitboards[pawn], f7);
    set_bit(bitboards[pawn], e7);
    set_bit(bitboards[pawn], d7);
    set_bit(bitboards[pawn], a7);
    set_bit(bitboards[pawn], b7);
    set_bit(bitboards[pawn], c7);

    set_bit(bitboards[KNIGHT], b1);
    set_bit(bitboards[KNIGHT], g1);
    set_bit(bitboards[knight], b8);
    set_bit(bitboards[knight], g8);

    set_bit(bitboards[BISHOP], c1);
    set_bit(bitboards[BISHOP], f1);
    set_bit(bitboards[bishop], c8);
    set_bit(bitboards[bishop], f8);

    set_bit(bitboards[ROOK], a1);
    set_bit(bitboards[ROOK], h1);
    set_bit(bitboards[rook], a8);
    set_bit(bitboards[rook], h8);

    set_bit(bitboards[QUEEN], d1);
    set_bit(bitboards[KING], e1);

    set_bit(bitboards[queen], d8);
    set_bit(bitboards[king], e8);
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    const std::string footer = "    a    b    c    d    e    f    g    h";
    const std::string emptySquare = "   ";

    std::string line = "  ";
    for(int i = 0; i < 8; i++)
        line += " ----";
    line += "\n";

    std::string output = line;

    for(int rank = 0; rank < 8; ++rank) {
        output += std::to_string(8 - rank) + " ";
        for(int file = 0; file < 8; ++file) {
            output += "| ";
            int square = rf_to_square(rank, file);

            int piece = -1;

            for(int i = 0; i < 12; i++) {
                if(get_bit(board.bitboards[i], square)) {
                    piece = i;
                    break;
                }
            }
            std::string pieceRepr = (piece == -1 ? " " : std::string(1, letter_pieces[piece]));
            output += get_bit(board.bitboards[piece], square) ? pieceRepr + "  " : emptySquare;
        }
        output += "|\n" + line;
    }

    os << "Turn: " << (!board.turn ? "White" : "Black") << std::endl;
    os << "Enpassant: " << (board.enpassant_square != -1 ? sq_to_coord[board.enpassant_square] : "xx");
    os << "\tCastling: " << (board.castling_rights & WK ? "WK " : "x") << (board.castling_rights & WQ ? "WQ " : "x")
       << (board.castling_rights & BK ? "BK " : "x") << (board.castling_rights & BQ ? "BQ " : "x") << std::endl;

    output += footer;
    os << output << std::endl;

    return os;
}

void Board::generate_moves() {
    int from, to;
    U64 bb, attacks;

    for (int piece = PAWN; piece < king; piece++) {
        bb = bitboards[piece];
        if (turn == W) {
            if (piece == PAWN) {
                while (bb) {
                    from = index_ls1b(bb);
                    to = from - 8;
                    attacks = get_pawn_attack_mask(W, from);
                    clear_bit(bb, from);
                }
            }
        } else {

        }
    }
}