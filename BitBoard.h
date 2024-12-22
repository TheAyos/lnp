#pragma once
#include <iostream>

#include "BitMove.h"
#include "BitOps.h"
#include "Definitions.h"
#include "Util.h"
#include "bitpieces/Pawn.h"

// TODO: replace with references where possible
class Board {
   public:
    U64 bitboards[12];
    U64 occupancies[3];
    int turn;
    int enpassant_square;
    int castling_rights;

    Board(bool initpieces);
    Board(const std::string &fen);
    void clear_board();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
    int get_piece_on_square(int square);
    BitMove parse_algebraic_move(int from, int to, char promotion_piece);
    void move(BitMove &move);

    BitMoveVec generate_moves();

   private:
    void setup_initial_pieces();
    void update_occupancies();

    void init_attacks();
    U64 pawn_attacks[2][64];
};

Board::Board(bool initpieces = true) {
    clear_board();
    init_attacks();

    if (initpieces)
        setup_initial_pieces();

    update_occupancies();
}

// https://www.chessprogramming.org/Forsyth-Edwards_Notation#Samples
// <FEN> ::=  <Piece Placement>
//        ' ' <Side to move>
//        ' ' <Castling ability>
//        ' ' <En passant target square>
//        ' ' <Halfmove clock>
//        ' ' <Fullmove counter>
Board::Board(const std::string &fen) : Board(false) {
    clear_board();

    int fen_index = 0;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rf_to_square(rank, file);

            // handles occupied squares
            if (fen[fen_index] >= 'A' && fen[fen_index] <= 'Z' || fen[fen_index] >= 'a' && fen[fen_index] <= 'z') {
                int piece = char_to_pieces[fen[fen_index]];
                set_bit(bitboards[piece], square);
                fen_index++;
            }

            // handles number of empty squares
            if (fen[fen_index] >= '0' && fen[fen_index] <= '9') {
                int offset = (int)(fen[fen_index] - '0');

                // fix well-known FEN parsing mistake
                if (get_piece_on_square(square) == -1)
                    file--;

                file += offset;
                fen_index++;
            }

            if (fen[fen_index] == '/')
                fen_index++;
        }
    }

    fen_index++;
    fen[fen_index] == 'w' ? turn = W : turn = B;

    fen_index++;
    fen_index++;

    while (fen[fen_index] != ' ') {
        // clang-format off
        switch (fen[fen_index]) {
            case 'K': castling_rights |= WK; break;
            case 'Q': castling_rights |= WQ; break;
            case 'k': castling_rights |= BK; break;
            case 'q': castling_rights |= BQ; break;
            case '-': break;
        }
        // clang-format on
        fen_index++;
    }

    fen_index++;
    if (fen[fen_index] == '-')
        enpassant_square = -1;
    else {
        enpassant_square = rf_to_square(fen[0] - 'a', 8 - (fen[1] - '0'));
    }

    update_occupancies();
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

void Board::update_occupancies() {
    for (int p = PAWN; p <= KING; p++)
        occupancies[W] |= bitboards[p];
    for (int p = pawn; p <= king; p++)
        occupancies[B] |= bitboards[p];

    occupancies[WB] |= occupancies[W] | occupancies[B];
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    const std::string footer = "    a    b    c    d    e    f    g    h";
    const std::string emptySquare = "   ";

    std::string line = "  ";
    for (int i = 0; i < 8; i++)
        line += " ----";
    line += "\n";

    std::string output = line;

    for (int rank = 0; rank < 8; ++rank) {
        output += std::to_string(8 - rank) + " ";
        for (int file = 0; file < 8; ++file) {
            output += "| ";
            int square = rf_to_square(rank, file);

            int piece = -1;

            for (int i = 0; i < 12; i++) {
                if (get_bit(board.bitboards[i], square)) {
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

int Board::get_piece_on_square(int square) {
    for (int p = PAWN; p <= king; p++)
        if (get_bit(bitboards[p], square))
            return p;
    return -1;
}

void Board::init_attacks() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[W][square] = Pawn::get_attack_mask(W, square);
        pawn_attacks[B][square] = Pawn::get_attack_mask(B, square);
    }
}

BitMove Board::parse_algebraic_move(int from, int to, char promotion_piece) {
    // use parser input to construct valid move object
    int piece = get_piece_on_square(from);
    bool capture = false;
    bool doublepush = false;
    bool enpassant = false;
    // TODO: complete handling all cases
    bool castling = false;

    if (piece == -1)
        Util::printDebug("[parse_algebraic_move] ERROR, HISTORY INVALID, NOT HANDLED, SHOULD NEVER HAPPEN");

    if (get_piece_on_square(to) != -1)
        capture = true;

    if (piece == PAWN || piece == pawn) {

        if (abs(to - from) == 16)
            doublepush = true;

        if (to % 8 != from % 8 && get_piece_on_square(to) == -1)
            enpassant = true;
    }

    return BitMove(from, to, piece, char_to_pieces[promotion_piece], capture, doublepush, enpassant, castling);
};

void Board::clear_board() {
    std::fill(std::begin(bitboards), std::end(bitboards), 0ULL);
    std::fill(std::begin(occupancies), std::end(occupancies), 0ULL);
    turn = W;
    enpassant_square = -1;
    castling_rights = 0;
};

void Board::move(BitMove &move) {
    // TODO: COPY/MAKE

    int from = move.get_from();
    int to = move.get_to();

    move_bit(bitboards[move.get_piece()], from, to);

    if (move.get_capture())
        ;

    if (move.get_promoted()) {
        clear_bit(bitboards[(turn == W) ? PAWN : pawn], to);
        set_bit(bitboards[move.get_promoted()], to);
    }

    if (move.get_enpassant())
        (turn == W) ? clear_bit(bitboards[pawn], to + 8) : clear_bit(bitboards[PAWN], to - 8);

    // a move is made, reset previous enpassant
    enpassant_square = -1;

    if (move.get_doublepush())
        (turn == W) ? (enpassant_square = to + 8) : (enpassant_square = to - 8);

    update_occupancies();
    turn = 1 - turn;
    // TODO: handle castling
    // TODO: handle checks
}

BitMoveVec Board::generate_moves() {
    BitMoveVec moves;
    int from, to;
    U64 bb, attacks;

    for (int piece = PAWN; piece < king; piece++) {

        // make a copy of the piece's bitboard (needed for the loop since we clear bits)
        bb = bitboards[piece];

        if (turn == W && piece == PAWN) {
            // forall pawns
            while (bb) {
                from = get_lsb_index(bb);
                // pawn attacks at least one square ahead
                to = (turn == W) ? from - 8 : from + 8;

                if (!(to < a8) && !get_bit(occupancies[WB], to)) {
                    // pawn promotion placement
                    if ((turn == W && from >= a7 && from <= h7) || (turn == B && from >= a2 && from <= h2)) {
                        moves.push_back(BitMove(from, to, piece, QUEEN, false, false, false, false));
                        moves.push_back(BitMove(from, to, piece, ROOK, false, false, false, false));
                        moves.push_back(BitMove(from, to, piece, BISHOP, false, false, false, false));
                        moves.push_back(BitMove(from, to, piece, KNIGHT, false, false, false, false));
                    } else {  // "normal" pawn moves
                        moves.push_back(BitMove(from, to, piece, NO_PROMOTION, false, false, false, false));
                        // double pawn push position and two squares ahead is free
                        if (((turn == W && from >= a2 && from <= h2) || turn == B && from >= a7 && from <= h7)
                            && !get_bit(occupancies[WB], turn == W ? to - 8 : to + 8)) {
                            moves.push_back(BitMove(
                                from, turn == W ? to - 8 : to + 8, piece, NO_PROMOTION, false, true, false, false));
                        }
                    }
                }

                // keep attacks that touch enemy pieces
                // std::cout << "WW" << std::endl;
                // display_bits(occupancies[W]);
                // std::cout << "BBB" << std::endl;
                // display_bits(occupancies[B]);
                // std::cout << "WBWBBW" << std::endl;
                // display_bits(occupancies[WB]);
                attacks = pawn_attacks[turn][from] & occupancies[1 - turn];
                // std::cout << "awnattacks" << std::endl;
                // display_bits(pawn_attacks[turn][from]);
                // display_bits(attacks);

                while (attacks) {
                    to = get_lsb_index(attacks);

                    // pawn promotion placement and not empty target (i.e. capture)
                    if ((turn == W && from >= a7 && from <= h7) || (turn == B && from >= a2 && from <= h2)) {
                        moves.push_back(BitMove(from, to, piece, QUEEN, true, false, false, false));
                        moves.push_back(BitMove(from, to, piece, ROOK, true, false, false, false));
                        moves.push_back(BitMove(from, to, piece, BISHOP, true, false, false, false));
                        moves.push_back(BitMove(from, to, piece, KNIGHT, true, false, false, false));
                    } else {
                        moves.push_back(BitMove(from, to, piece, NO_PROMOTION, true, false, false, false));
                    }

                    // remove attack (loop)
                    clear_bit(attacks, to);
                }

                std::cout << sq_to_coord[enpassant_square] << std::endl;
                if (enpassant_square != -1) {
                    std::cout << "FHZIOEOIAZFHOAZIFHAZ" << std::endl;
                    U64 enpassant_attacks = pawn_attacks[turn][from] & (1ULL << enpassant_square);
                    display_bits(pawn_attacks[turn][from]);
                    display_bits(enpassant_attacks);
                    display_bits(enpassant_attacks);
                    display_bits(enpassant_attacks);

                    if (enpassant_attacks) {
                        int enpassant_to = get_lsb_index(enpassant_attacks);
                        moves.push_back(BitMove(from, enpassant_to, piece, NO_PROMOTION, true, false, true, false));
                    }
                }

                // remove piece from bitboard (loop)
                clear_bit(bb, from);
            }
        }
    }

    Util::printDebug("generated " + std::to_string(moves.size()) + " moves");
    return moves;
}