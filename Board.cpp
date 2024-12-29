#include "Board.h"

#include <chrono>
#include <cstring>
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

// starting FEN position
Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"){};

// TODO: FIXME: OPTI: fixcpcpcpc
Board::Board(const std::string &fen) {
    // https://www.chessprogramming.org/Forsyth-Edwards_Notation#Samples
    // <FEN> ::=  <Piece Placement>
    //        ' ' <Side to move>
    //        ' ' <Castling ability>
    //        ' ' <En passant target square>
    //        ' ' <Halfmove clock>
    //        ' ' <Fullmove counter>
    clear_board();
    init_attacks();

    // interesting : ++i increments i before using its value, i++ increments i after using its value
    size_t fen_index = 0;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rf_to_square(rank, file);

            // handles occupied squares
            if (fen[fen_index] >= 'A' && fen[fen_index] <= 'Z' || fen[fen_index] >= 'a' && fen[fen_index] <= 'z') {
                int piece = char_to_pieces(fen[fen_index++]);
                set_bit(bitboards[piece], square);
            }

            // handles number of empty squares
            if (fen[fen_index] >= '0' && fen[fen_index] <= '9') {
                int offset = (fen[fen_index++] - '0');

                // fix well-known FEN parsing mistake
                if (get_piece_on_square(square) == -1) file--;

                file += offset;
            }

            if (fen[fen_index] == '/') fen_index++;
        }
    }

    fen[++fen_index] == 'w' ? turn = W : turn = B;

    fen_index++;
    while (fen[++fen_index] != ' ') {
        switch (fen[fen_index]) {
            case 'K': castlingRights |= WK; break;
            case 'k': castlingRights |= BK; break;
            case 'Q': castlingRights |= WQ; break;
            case 'q': castlingRights |= BQ; break;
            case '-': break;
            default: Util::exitError("[Board::Board(fen)]INVALID FEN STRING"); break;
        }
    }
    enpassantSquare = (fen[++fen_index] == '-') ? -1 : rf_to_square(fen[0] - 'a', 8 - (fen[1] - '0'));

    // important to init board !!
    update_occupancies();
}

// void Board::setup_initial_pieces() {
//     set_bit(bitboards[PAWN], g2);
//     set_bit(bitboards[PAWN], h2);
//     set_bit(bitboards[PAWN], f2);
//     set_bit(bitboards[PAWN], e2);
//     set_bit(bitboards[PAWN], d2);
//     set_bit(bitboards[PAWN], a2);
//     set_bit(bitboards[PAWN], b2);
//     set_bit(bitboards[PAWN], c2);
//     set_bit(bitboards[pawn], g7);
//     set_bit(bitboards[pawn], h7);
//     set_bit(bitboards[pawn], f7);
//     set_bit(bitboards[pawn], e7);
//     set_bit(bitboards[pawn], d7);
//     set_bit(bitboards[pawn], a7);
//     set_bit(bitboards[pawn], b7);
//     set_bit(bitboards[pawn], c7);
//     set_bit(bitboards[KNIGHT], b1);
//     set_bit(bitboards[KNIGHT], g1);
//     set_bit(bitboards[knight], b8);
//     set_bit(bitboards[knight], g8);
//     set_bit(bitboards[BISHOP], c1);
//     set_bit(bitboards[BISHOP], f1);
//     set_bit(bitboards[bishop], c8);
//     set_bit(bitboards[bishop], f8);
//     set_bit(bitboards[ROOK], a1);
//     set_bit(bitboards[ROOK], h1);
//     set_bit(bitboards[rook], a8);
//     set_bit(bitboards[rook], h8);
//     set_bit(bitboards[QUEEN], d1);
//     set_bit(bitboards[KING], e1);
//     set_bit(bitboards[queen], d8);
//     set_bit(bitboards[king], e8);
// }

void Board::update_occupancies() {
    // learned that fill is not the best way to set all bits to 0 for contiguous memory
    // it is more for containers that do not have contiguous memory, like lists/vectors
    // with custom allocators
    std::fill(std::begin(occupancies), std::end(occupancies), 0ULL);
    // OPTI: still seems to be the same speed ?
    // std::memset(occupancies, 0, sizeof(occupancies));

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
    // OPTI: can i fill from bitboards to occupancies ? aren't they contiguous in memory ?
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
    // TODO: ?: add sanity checks for board state variables somewhere?
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

#define DEBUG 0

int Board::move(const BitMove &move, bool justCheckCheck) {
    // OPTI: explore using stacks OR making an unmake_move function for undoing moves
    // save board state for potential undo at end of function
    BoardState savedState(*this);

    int from = move.get_from();
    int to = move.get_to();
    if (DEBUG)
        std::cout << "[Board::move] Trying to move :" << sq_to_coord(from) << sq_to_coord(to)
                  << letter_pieces[move.get_promotion_piece()] << std::endl;

    /* ----------------------------- regular capture ---------------------------- */
    // OPTI: minimal: variation of get_piece_on_square with enemy color to avoid iterating over 6 bitboards
    if (move.get_capture()) {
        if (DEBUG) std::cout << "!!!capturing piece: " << letter_pieces[get_piece_on_square(to)] << std::endl;
        // if (DEBUG) std::cout << "-*---*-BEFORE CAPTURE HANDLING-*---*-" << *this << std::endl;
        clear_bit(bitboards[get_piece_on_square(to)], to);
        // if (DEBUG) std::cout << "-*---*-AFTER CAPTURE HANDLING-*---*-" << *this << std::endl;
    }

    /* ------------------------------- move piece ------------------------------- */
    // if (DEBUG) std::cout << "-*---*-BEFORE MOVEBIT HANDLING-*---*-" << *this << std::endl;
    move_bit(bitboards[move.get_piece()], from, to);
    // if (DEBUG) std::cout << "-*---*-AFTER MOVEBIT HANDLING-*---*-" << *this << std::endl;

    /* ------------------------------- en passant ------------------------------- */
    if (move.get_enpassant()) (turn == W) ? clear_bit(bitboards[pawn], to + 8) : clear_bit(bitboards[PAWN], to - 8);

    // a move is made, reset previous enpassant
    enpassantSquare = -1;

    /* ---------------------------- double pawn push ---------------------------- */
    if (move.get_doublepush()) (turn == W) ? (enpassantSquare = to + 8) : (enpassantSquare = to - 8);

    /* ----------------------------- pawn promotion ----------------------------- */
    if (move.get_promotion_piece()) {
        clear_bit(bitboards[get_color_piece(PAWN, turn)], to);
        set_bit(bitboards[get_color_piece(move.get_promotion_piece(), turn)], to);
    }

    /* ------------------------------ king castling ----------------------------- */
    if (move.get_castling()) {
        // handle castling according to the castling type (in order: WK, WQ, BK, BQ)
        switch (to) {
            case g1: move_bit(bitboards[ROOK], h1, f1); break;
            case c1: move_bit(bitboards[ROOK], a1, d1); break;
            case g8: move_bit(bitboards[rook], h8, f8); break;
            case c8: move_bit(bitboards[rook], a8, d8); break;
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
    if (DEBUG) std::cout << *this;

    if (is_attacked(find_king(player), enemy)) {
        if (DEBUG)
            std::cout << "[Board::move] INVALID MOVE, " << ((player == W) ? "WHITE" : "BLACK")
                      << " KING IN CHECK :" << sq_to_coord(from) << "->" << sq_to_coord(to) << std::endl;
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

bool Board::is_attacked(int sq, int by_color) {
    // threatened by leaper pieces
    if (pawnAttacks[1 - by_color][sq] & bitboards[get_color_piece(PAWN, by_color)]) return true;
    if (knightAttacks[sq] & bitboards[get_color_piece(KNIGHT, by_color)]) return true;
    if (kingAttacks[sq] & bitboards[get_color_piece(KING, by_color)]) return true;

    // threatened by sliding pieces
    if (Bishop::get_attack_masks_blocking(sq, occupancies[WB]) & bitboards[get_color_piece(BISHOP, by_color)])
        return true;
    if (Rook::get_attack_masks_blocking(sq, occupancies[WB]) & bitboards[get_color_piece(ROOK, by_color)]) return true;
    if (Queen::get_attack_masks_blocking(sq, occupancies[WB]) & bitboards[get_color_piece(QUEEN, by_color)])
        return true;

    return false;
}

void Board::add_move_if_legal(BitMoveVec &moveVec, const BitMove &m) {
    int moveLeadsToCheck = move(m, true);
    if (!moveLeadsToCheck) moveVec.push_back(m);
};

BitMoveVec Board::get_all_legal_moves() {
    BitMoveVec moves;

    Pawn::add_legal_moves(*this, moves);
    Knight::add_legal_moves(*this, moves);
    Bishop::add_legal_moves(*this, moves);
    Rook::add_legal_moves(*this, moves);
    Queen::add_legal_moves(*this, moves);
    King::add_legal_moves(*this, moves);

    // OPTI: crucial: use pre-allocated array instead of vector for performance
    // Util::printDebug("[Board::FFSIZE]" + std::to_string(moves.size()));
    // Util::printDebug("[Board::get_all_legal_moves] generated " + std::to_string(moves.size()) + " moves");
    return moves;
}

void Board::perftree(int depth) {
    BitMoveVec moves = get_all_legal_moves();
    long long totalNodes = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (const BitMove &mv : moves) {

        BoardState savedState(*this);
        if (move(mv) == -1) continue;
        long nodes = perft_search(depth - 1);
        savedState.reapply(*this);

        std::cout << mv.get_algebraic_notation() << " " << nodes << std::endl;
        totalNodes += nodes;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << std::endl << totalNodes << std::endl;
}

long Board::perft_search(int depth) {

    long res = 0;
    if (depth == 0) return 1;
    BitMoveVec moves = get_all_legal_moves();

    for (const BitMove &mv : moves) {

        BoardState savedState(*this);

        if (move(mv) == -1) continue;
        res += perft_search(depth - 1);

        savedState.reapply(*this);
    }
    return res;
}
