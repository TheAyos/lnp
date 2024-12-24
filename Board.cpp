#include "Board.h"

#include <iostream>

#include "BitMove.h"
#include "BitOps.h"
#include "Definitions.h"
#include "bitpieces/Bishop.h"
#include "bitpieces/King.h"
#include "bitpieces/Knight.h"
#include "bitpieces/Pawn.h"
#include "bitpieces/Queen.h"
#include "bitpieces/Rook.h"

using namespace BitOps;

/* -------------------------------------------------------------------------- */
/*                         constructor & board-related                        */
/* -------------------------------------------------------------------------- */

Board::Board(bool initpieces) {
    clear_board();
    init_attacks();
    if (initpieces) setup_initial_pieces();
    update_occupancies();
}

Board::Board(const std::string &fen) : Board(false) {
    // initpieces is set to false here since we will set the pieces from the FEN string,
    //               and then call the main constructor to continue the initizaliations

    // https://www.chessprogramming.org/Forsyth-Edwards_Notation#Samples
    // <FEN> ::=  <Piece Placement>
    //        ' ' <Side to move>
    //        ' ' <Castling ability>
    //        ' ' <En passant target square>
    //        ' ' <Halfmove clock>
    //        ' ' <Fullmove counter>
    clear_board();

    int fen_index = 0;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rf_to_square(rank, file);

            // handles occupied squares
            if (fen[fen_index] >= 'A' && fen[fen_index] <= 'Z' || fen[fen_index] >= 'a' && fen[fen_index] <= 'z') {
                int piece = char_to_pieces(fen[fen_index]);
                set_bit(bitboards[piece], square);
                fen_index++;
            }

            // handles number of empty squares
            if (fen[fen_index] >= '0' && fen[fen_index] <= '9') {
                int offset = (int)(fen[fen_index] - '0');

                // fix well-known FEN parsing mistake
                if (get_piece_on_square(square) == -1) file--;

                file += offset;
                fen_index++;
            }

            if (fen[fen_index] == '/') fen_index++;
        }
    }

    fen_index++;
    fen[fen_index] == 'w' ? turn = W : turn = B;

    fen_index++;
    fen_index++;

    while (fen[fen_index] != ' ') {
        // clang-format off
        switch (fen[fen_index]) {
            case 'K': castlingRights |= WK; break;
            case 'Q': castlingRights |= WQ; break;
            case 'k': castlingRights |= BK; break;
            case 'q': castlingRights |= BQ; break;
            case '-': break;
        }
        // clang-format on
        fen_index++;
    }

    fen_index++;
    if (fen[fen_index] == '-')
        enpassantSquare = -1;
    else {
        enpassantSquare = rf_to_square(fen[0] - 'a', 8 - (fen[1] - '0'));
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

void Board::init_attacks() {
    for (int square = 0; square < 64; square++) {
        pawnAttacks[W][square] = Pawn::get_attack_mask(W, square);
        pawnAttacks[B][square] = Pawn::get_attack_mask(B, square);

        knightAttacks[square] = Knight::get_attack_mask(square);
        kingAttacks[square] = King::get_attack_mask(square);
    }
}

void Board::clear_board() {
    std::fill(std::begin(bitboards), std::end(bitboards), 0ULL);
    std::fill(std::begin(occupancies), std::end(occupancies), 0ULL);
    turn = W;
    enpassantSquare = -1;
    castlingRights = 0;
};

/* -------------------------------------------------------------------------- */
/*                              helper functions                              */
/* -------------------------------------------------------------------------- */

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
    os << "Enpassant: " << (board.enpassantSquare != -1 ? sq_to_coord(board.enpassantSquare) : "xx");
    os << "\tCastling: " << (board.castlingRights & WK ? "WK " : "x") << (board.castlingRights & WQ ? "WQ " : "x")
       << (board.castlingRights & BK ? "BK " : "x") << (board.castlingRights & BQ ? "BQ " : "x") << std::endl;

    output += footer;
    os << output << std::endl;

    return os;
}

int Board::get_piece_on_square(int square) {
    for (int p = PAWN; p <= king; p++)
        if (get_bit(bitboards[p], square)) return p;
    return -1;
}

bool Board::is_empty(int square) {
    return !get_bit(occupancies[WB], square);
}

int Board::get_color_piece(int piece, int color) {
    if (piece >= PAWN && piece <= KING) {
        return (color == W) ? piece : piece + 6;
    } else if (piece >= pawn && piece <= king) {
        return (color == W) ? piece - 6 : piece;
    }
    return piece;
}

int Board::find_king(int color) {
    return get_lsb_index(bitboards[get_color_piece(KING, color)]);
}

/* -------------------------------------------------------------------------- */
/*                                move parsing                                */
/* -------------------------------------------------------------------------- */

BitMove Board::parse_algebraic_move(int from, int to, char promotion_piece) {
    int piece = get_piece_on_square(from);
    bool capture = false;
    bool doublepush = false;
    bool enpassant = false;
    bool castling = false;

    if (piece == -1)
        Util::printDebug(
            "[parse_algebraic_move] ERROR, HISTORY INVALID, NOT HANDLED, SHOULD NEVER HAPPEN...PANIC!!!!!!!!");

    if (get_piece_on_square(to) != -1) capture = true;

    if (piece == PAWN || piece == pawn) {

        // detect pawn doublepush
        if (abs(to - from) == 16) doublepush = true;

        // detect pawn enpassant opportunity
        if (to % 8 != from % 8 && get_piece_on_square(to) == -1) enpassant = true;
    }

    return BitMove(from, to, piece, char_to_pieces(promotion_piece), capture, doublepush, enpassant, castling);
};

int Board::move(const BitMove &move, bool justCheckCheck) {
    // TODO: handle checks
    // TODO: handle undoing

    // save board state for potential undo at end of function
    BoardState savedState(*this);

    int from = move.get_from();
    int to = move.get_to();
    std::cout << "[Board::move] Trying making the move :" << sq_to_coord(from) << sq_to_coord(to) << std::endl;

    /* ------------------------------- move piece ------------------------------- */

    move_bit(bitboards[move.get_piece()], from, to);

    /* ----------------------------- regular capture ---------------------------- */

    // OPTI: minimal: variation of get_piece_on_square with enemy color to avoid iterating over 6 bitboards
    if (move.get_capture()) {
        std::cout << "-*---*-BEFORE CAPTURE HANDLING-*---*-" << *this << std::endl;
        clear_bit(bitboards[get_piece_on_square(to)], to);
        std::cout << "-*---*-AFTER CAPTURE HANDLING-*---*-" << *this << std::endl;
    }

    /* ------------------------------- en passant ------------------------------- */

    if (move.get_enpassant()) (turn == W) ? clear_bit(bitboards[pawn], to + 8) : clear_bit(bitboards[PAWN], to - 8);

    // a move is made, reset previous enpassant
    enpassantSquare = -1;

    /* ---------------------------- double pawn push ---------------------------- */

    if (move.get_doublepush()) (turn == W) ? (enpassantSquare = to + 8) : (enpassantSquare = to - 8);

    /* ----------------------------- pawn promotion ----------------------------- */

    if (move.get_promotion_piece()) {
        clear_bit(bitboards[get_color_piece(PAWN, turn)], to);
        set_bit(bitboards[move.get_promotion_piece()], to);
    }

    /* ------------------------------ king castling ----------------------------- */

    if (move.get_castling()) {
        // handle castling according to the castling type (in order: WK, WQ, BK, BQ)
        switch (to) {
            case g1:
                move_bit(bitboards[ROOK], h1, f1);
                break;
            case c1:
                move_bit(bitboards[ROOK], a1, d1);
                break;
            case g8:
                move_bit(bitboards[rook], h8, f8);
                break;
            case c8:
                move_bit(bitboards[rook], a8, d8);
                break;
        }
    }

    // update castling rights according to the move, using precalculated masks
    castlingRights &= castlingRightsMasks[from] & castlingRightsMasks[to];

    // update board state
    update_occupancies();
    int player = turn;
    int enemy = 1 - turn;
    turn = 1 - turn;

    // cHeCkInG cHeCkS

    // std::cout << sq_to_coord(find_king(player)) << std::endl;
    // std::cout << sq_to_coord(BitOps::get_lsb_index(bitboards[player == W ? KING : king])) << std::endl;
    // std::cout << is_attacked(find_king(player), enemy) << std::endl;
    // std::cout << is_attacked(BitOps::get_lsb_index(bitboards[player == W ? KING : king]), enemy) << std::endl;

    if (is_attacked(find_king(player), enemy)) {
        std::cout << "[Board::move] INVALID MOVE, KING IN CHECK :" << sq_to_coord(from) << "->" << sq_to_coord(to)
                  << std::endl;
        savedState.reapply(*this);
        return -1;  // illegal
    }

    // restore board state if we only wanted to check for checks and not apply the move
    if (justCheckCheck) savedState.reapply(*this);

    return 0;  // legal
}

/* -------------------------------------------------------------------------- */
/*                               attack-related                               */
/* -------------------------------------------------------------------------- */

// bool Board::is_attacked(int sq, int by_color) {
//     // threatened by leaper pieces
//     if (pawnAttacks[1 - by_color][sq] & bitboards[get_color_piece(PAWN, by_color)]) return true;
//     if (knightAttacks[sq] & bitboards[get_color_piece(KNIGHT, by_color)]) return true;
//     if (kingAttacks[sq] & bitboards[get_color_piece(KING, by_color)]) return true;

//     // threatened by sliding pieces
//     if (Bishop::get_attack_masks_blocking(sq, occupancies[WB]) & bitboards[get_color_piece(BISHOP, by_color)])
//         return true;
//     if (Rook::get_attack_masks_blocking(sq, occupancies[WB]) & bitboards[get_color_piece(ROOK, by_color)]) return
//     true; if (Queen::get_attack_masks_blocking(sq, occupancies[WB]) & bitboards[get_color_piece(QUEEN, by_color)])
//         return true;

//     return false;
// }

bool Board::is_attacked(int square, int by_turn) {

    // threatened by leaper pieces
    if (pawnAttacks[1 - by_turn][square] & bitboards[(by_turn == W) ? PAWN : pawn]) return true;
    if (knightAttacks[square] & bitboards[(by_turn == W) ? KNIGHT : knight]) return true;
    if (kingAttacks[square] & bitboards[(by_turn == W) ? KING : king]) return true;

    // threatened by sliding pieces
    int piece;

    piece = (by_turn == W) ? BISHOP : bishop;
    if (Bishop::get_attack_masks_blocking(square, occupancies[WB]) & bitboards[piece]) return true;

    piece = (by_turn == W) ? ROOK : rook;
    if (Rook::get_attack_masks_blocking(square, occupancies[WB]) & bitboards[piece]) return true;

    piece = (by_turn == W) ? QUEEN : queen;
    if (Queen::get_attack_masks_blocking(square, occupancies[WB]) & bitboards[piece]) return true;

    return false;
}

void Board::add_move_if_legal(BitMoveVec &moveVec, const BitMove &m) {
    // if (!is_attacked(m.get_to(), 1 - turn))
    //     std::cout << "piece " << letter_pieces[get_piece_on_square(m.get_from())] << " is safe in square " <<
    //     sq_to_coord(m.get_from())
    //               << std::endl;

    // only skip if has choice (don't skip if this resolves a check)
    // if (is_attacked(m.get_to(), 1 - turn)) {
    //     if (m.get_to() == e7) {
    //         printf("\n");
    //         for (int rank = 0; rank < 8; rank++) {
    //             for (int file = 0; file < 8; file++) {
    //                 if (!file) printf("  %d ", 8 - rank);
    //                 printf(" %d", is_attacked(BitOps::rf_to_square(rank, file), 1 - turn) ? 1 : 0);
    //             }
    //             printf("\n");
    //         }
    //         printf("\n     a b c d e f g h\n\n");
    //     }
    //     std::cout << "skip illegal(capturing) move: " << sq_to_coord(m.get_from()) << "->" << sq_to_coord(m.get_to())
    //               << std::endl;
    //     return;
    // }

    // can only check for this in make move !!! need to update whole board & restor!!
    // if (is_attacked(get_lsb_index(bitboards[get_color_piece(KING, turn)]), 1 - turn)) {

    //     printf("\n");
    //     for (int rank = 0; rank < 8; rank++) {
    //         for (int file = 0; file < 8; file++) {
    //             if (!file) printf("  %d ", 8 - rank);
    //             printf(" %d", is_attacked(BitOps::rf_to_square(rank, file), 1-turn) ? 1 : 0);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n     a b c d e f g h\n\n");

    //     std::cout << "skip illegal(checking) move: " << sq_to_coord(m.get_from()) << "->" << sq_to_coord(m.get_to())
    //               << std::endl;
    //     return;
    // }
    int moveLeadsToCheck = move(m, true);
    if (!moveLeadsToCheck) moveVec.push_back(m);
    // if (this->move(m) != -1) moveVec.push_back(m);
    // else {
    //     std::cout << "[Board::add_move_if_legal] DROPPING ILLEGAL MOVE, KING IN CHECK :" << sq_to_coord(m.get_from())
    //     << "->" << sq_to_coord(m.get_to()) << std::endl;
    // }
    // std::cout << this;
};

// TODO: make these pseudolegal moves legal
BitMoveVec Board::get_all_legal_moves() {
    BitMoveVec moves;
    // FIXME: need to dereference the pointer !!!
    Pawn::add_legal_moves(*this, moves);
    Knight::add_legal_moves(*this, moves);
    Bishop::add_legal_moves(*this, moves);
    Rook::add_legal_moves(*this, moves);
    Queen::add_legal_moves(*this, moves);
    King::add_legal_moves(*this, moves);

    Util::printDebug("[Board::get_all_legal_moves] generated " + std::to_string(moves.size()) + " moves");
    return moves;
}