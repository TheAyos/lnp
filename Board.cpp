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
// #include "Zobrist.h"

using namespace BitOps;

/* -------------------------------------------------------------------------- */
/*                         constructor & board-related                        */
/* -------------------------------------------------------------------------- */

// starting FEN position
Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
    for (int i = 4; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            int idx = i * 8 + j;
            if (i == 7) {
                if (j == 0 || j == 7) pieceOnSquare[idx] = 3;
                if (j == 1 || j == 6) pieceOnSquare[idx] = 1;
                if (j == 2 || j == 5) pieceOnSquare[idx] = 2;
                if (j == 3) pieceOnSquare[idx] = 4;
                if (j == 4) pieceOnSquare[idx] = 5;
            } else if (i == 6)
                pieceOnSquare[idx] = 0;
            else
                pieceOnSquare[idx] = 12;

            int ridx = (7 - i) * 8 + j;
            if (i >= 6)
                pieceOnSquare[ridx] = pieceOnSquare[idx] + 6;
            else
                pieceOnSquare[ridx] = 12;
        }

    for(int i = 0; i < 512; i++) {
	this->history[i].ep = -1;
	this->history[i].hash = 0;
    }

    for (int i = 0; i < 64; i++ ) {
	if (pieceOnSquare[i] < 12) {
	    int color = (i < 6) ? W : B;
	    Zobrist::flip_sq(this->history[0].hash, i, pieceOnSquare[i]%6, color);
	}
    }

    ply = 0;
};

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

U64 Board::get_hash() const noexcept { return this->history[ply].hash; }

bool Board::isThreefold() const noexcept {
    for (size_t i = 0; i < ply; i++)
	if (history[i].hash == history[ply].hash) return true;
    return false;
}

void Board::update_occupancies() {
    // learned that fill is not the best way to set all bits to 0 for contiguous memory
    // it is more for containers that do not have contiguous memory, like lists/vectors
    // with custom allocators
    //std::fill(std::begin(occupancies), std::end(occupancies), 0ULL);
    std::memset(occupancies, 0, sizeof(occupancies));

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
        rookSlidingMasks[square] = Rook::get_sliding_mask(square);
        bishopSlidingMasks[square] = Bishop::get_sliding_mask(square);
    }
    Rook::initMagics(*this);
    Bishop::initMagics(*this);
}

void Board::clear_board() {
    // OPTI: can i fill from bitboards to occupancies ? aren't they contiguous in memory ?
    std::fill(std::begin(bitboards), std::end(bitboards), 0ULL);
    std::fill(std::begin(occupancies), std::end(occupancies), 0ULL);
    turn = W;
    ply = 0;
    enpassantSquare = -1;
    castlingRights = 0;
    for (int i = 0; i < 64; i++)
        pieceOnSquare[i] = 12;
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
    os << "Enpassant: " << (board.enpassantSquare != -1 ? _sq_to_coord[board.enpassantSquare] : "xx");
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

bool Board::playerInCheck(int color) {
    return is_attacked(find_king(color), 1 - color);
}

/* -------------------------------------------------------------------------- */
/*                                move parsing                                */
/* -------------------------------------------------------------------------- */

#define DEBUG 0

int Board::make_move(const BitMove &move, bool justCheckCheck, bool onlyCapture) {

    if (onlyCapture) {
        if (move.get_capture())
            make_move(move, justCheckCheck, false);
        else
            return 0;
    }

    // OPTI: explore using stacks OR making an unmake_move function for undoing moves
    // save board state for potential undo at end of function
    BoardState savedState(*this);

    int from = move.get_from();
    int to = move.get_to();

    // int captured = to;
    int captured_piece = pieceOnSquare[to];
    // std::cout << pieceOnSquare[from] << std::endl;
    pieceOnSquare[to] = pieceOnSquare[from];
    pieceOnSquare[from] = 12;
    int new_piece = pieceOnSquare[to]%6;

    // zobrist init
    ply++;
    history[ply].hash = history[ply-1].hash;
    
    Zobrist::flip_sq(this->history[ply].hash, from, new_piece, turn); // remove old piece

    if (DEBUG)
        std::cout << "[Board::move] Trying to move :" << _sq_to_coord[from] << _sq_to_coord[to]
                  << letter_pieces[move.get_promotion_piece()] << std::endl;

    /* ----------------------------- regular capture ---------------------------- */
    // OPTI: minimal: variation of get_piece_on_square with enemy color to avoid iterating over 6 bitboards
    if (move.get_capture()) {
        if (DEBUG) std::cout << "!!!capturing piece: " << letter_pieces[get_piece_on_square(to)] << std::endl;
        // if (DEBUG) std::cout << "-*---*-BEFORE CAPTURE HANDLING-*---*-" << *this << std::endl;
        clear_bit(bitboards[captured_piece], to);
        // if (DEBUG) std::cout << "-*---*-AFTER CAPTURE HANDLING-*---*-" << *this << std::endl;
	Zobrist::flip_sq(this->history[ply].hash, to, captured_piece%6, turn);
    }

    /* ------------------------------- move piece ------------------------------- */
    // if (DEBUG) std::cout << "-*---*-BEFORE MOVEBIT HANDLING-*---*-" << *this << std::endl;
    move_bit(bitboards[move.get_piece()], from, to);
    // if (DEBUG) std::cout << "-*---*-AFTER MOVEBIT HANDLING-*---*-" << *this << std::endl;

    /* ------------------------------- en passant ------------------------------- */
    if (move.get_enpassant()) {
        if (turn == W) {
            clear_bit(bitboards[pawn], to + 8);
            pieceOnSquare[to + 8] = 12;
        } else {
            clear_bit(bitboards[PAWN], to - 8);
            pieceOnSquare[to - 8] = 12;
        }
    }

    // a move is made, reset previous enpassant
    enpassantSquare = -1;

    /* ---------------------------- double pawn push ---------------------------- */
    if (move.get_doublepush()) (turn == W) ? (enpassantSquare = to + 8) : (enpassantSquare = to - 8);

    /* ----------------------------- pawn promotion ----------------------------- */
    if (move.get_promotion_piece()) {
        clear_bit(bitboards[get_color_piece(PAWN, turn)], to);
        set_bit(bitboards[get_color_piece(move.get_promotion_piece(), turn)], to);

        pieceOnSquare[to] = get_color_piece(move.get_promotion_piece(), turn);
	new_piece = pieceOnSquare[to]%6;
    }

    /* ------------------------------ king castling ----------------------------- */
    if (move.get_castling()) {
        // handle castling according to the castling type (in order: WK, WQ, BK, BQ)
        switch (to) {
            case g1:
                move_bit(bitboards[ROOK], h1, f1);
                pieceOnSquare[61] = pieceOnSquare[63];
                pieceOnSquare[63] = 12;
		Zobrist::flip_sq(this->history[ply].hash, 61, ROOK, turn);
		Zobrist::flip_sq(this->history[ply].hash, 63, ROOK, turn);
                break;
            case c1:
                move_bit(bitboards[ROOK], a1, d1);
                pieceOnSquare[59] = pieceOnSquare[56];
                pieceOnSquare[56] = 12;
		Zobrist::flip_sq(this->history[ply].hash, 59, ROOK, turn);
		Zobrist::flip_sq(this->history[ply].hash, 56, ROOK, turn);
                break;
            case g8:
                move_bit(bitboards[rook], h8, f8);
                pieceOnSquare[5] = pieceOnSquare[7];
                pieceOnSquare[7] = 12;
		Zobrist::flip_sq(this->history[ply].hash, 5, ROOK, turn);
		Zobrist::flip_sq(this->history[ply].hash, 7, ROOK, turn);
                break;
            case c8:
                move_bit(bitboards[rook], a8, d8);
                pieceOnSquare[3] = pieceOnSquare[0];
                pieceOnSquare[0] = 12;
		Zobrist::flip_sq(this->history[ply].hash, 3, ROOK, turn);
		Zobrist::flip_sq(this->history[ply].hash, 0, ROOK, turn);
                break;
        }
    }
    
    // the new piece on the destination square
    // std::cout << "CURRENT TURN " << turn << std::endl; 
    Zobrist::flip_sq(this->history[ply].hash, to, new_piece, turn); // add the new piece

    // update castling rights according to the move, using precalculated masks
    castlingRights &= castlingRightsMasks[from] & castlingRightsMasks[to];

    // update board state
    update_occupancies();
    int player = turn;
    int enemy = 1 - turn;
    turn = 1 - turn;
    history[ply].ep = enpassantSquare;
    // history[ply].castling = castlingRights;
    
    
    if (auto p_ep = this->history[ply-1].ep; p_ep != enpassantSquare) {
	Zobrist::flip_ep(this->history[ply].hash, enpassantSquare);
	Zobrist::flip_ep(this->history[ply].hash, p_ep);
    }
    
    Zobrist::flip_color(this->history[ply].hash);
    move_count++;


    if (DEBUG) std::cout << *this;

    if (playerInCheck(player)) {
        if (DEBUG)
            std::cout << "[Board::move] INVALID MOVE, " << ((player == W) ? "WHITE" : "BLACK")
                      << " KING IN CHECK :" << _sq_to_coord[from] << "->" << _sq_to_coord[to] << std::endl;
        savedState.reapply(*this);
        return -1;  // illegal
    }

    // restore board state if we only wanted to check for checks and not apply the move
    if (justCheckCheck) savedState.reapply(*this);

    return 0;  // legal
}

/* ------------------------------------------------------------------------ */
/*                               attack-related                               */
/* -------------------------------------------------------------------------- */

bool Board::is_attacked(int sq, int by_color) {
    // threatened by leaper pieces
    if (pawnAttacks[1 - by_color][sq] & bitboards[get_color_piece(PAWN, by_color)]) return true;
    if (knightAttacks[sq] & bitboards[get_color_piece(KNIGHT, by_color)]) return true;
    if (kingAttacks[sq] & bitboards[get_color_piece(KING, by_color)]) return true;

    // threatened by sliding pieces
    if (Bishop::get_attack_masks_blocking_magic(*this,sq, occupancies[WB]) & bitboards[get_color_piece(BISHOP, by_color)])
        return true;
    if (Rook::get_attack_masks_blocking_magic(*this, sq, occupancies[WB]) & bitboards[get_color_piece(ROOK, by_color)]) return true;
    if (Queen::get_attack_masks_blocking(*this, sq, occupancies[WB]) & bitboards[get_color_piece(QUEEN, by_color)])
        return true;

    return false;
}

/*
template <MoveList::Type T>
void Board::add_move_if_legal(MoveList &moves, const BitMove &m) {
    int moveLeadsToCheck = make_move(m, true);
    if (!moveLeadsToCheck) moves.push<T>(m);
    // moveVec.emplace_back(m); // optimization
}*/

MoveList Board::get_all_legal_moves() {
    MoveList moves;
    // OPTI: even more, can use a preallocated array
    // moves.reserve(256);

    Pawn::add_legal_moves(*this, moves);
    Knight::add_legal_moves(*this, moves);
    Bishop::add_legal_moves(*this, moves);
    Rook::add_legal_moves(*this, moves);
    Queen::add_legal_moves(*this, moves);
    King::add_legal_moves(*this, moves);

    moves.finish();

    // Util::printDebug("[Board::FFSIZE]" + std::to_string(moves.size()));
    // Util::printDebug("[Board::get_all_legal_moves] generated " + std::to_string(moves.size()) + " moves");
    return moves;
}

MoveList Board::get_capture_moves() {
    MoveList captureMoves;
    // captureMoves.reserve(256);

    Pawn::add_legal_moves(*this, captureMoves, true);
    Knight::add_legal_moves(*this, captureMoves, true);
    Bishop::add_legal_moves(*this, captureMoves, true);
    Rook::add_legal_moves(*this, captureMoves, true);
    Queen::add_legal_moves(*this, captureMoves, true);
    King::add_legal_moves(*this, captureMoves, true);

    captureMoves.finish();

    return captureMoves;
}

void Board::perftree(int depth) {
    MoveList moves = get_all_legal_moves();
    long long totalNodes = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (const BitMove &mv : moves) {

        BoardState savedState(*this);
        if (make_move(mv) == -1) continue;
        long nodes = perft_search(depth - 1);
        savedState.reapply(*this);

        std::cout << mv.get_algebraic_notation() << " " << nodes << std::endl;
        totalNodes += nodes;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << std::endl << totalNodes << " time " << duration << std::endl;
}

// was testing speed below
// inline int evaluate_piece(int piece) {
//     if (piece == -1) return 0;
//     int p_evaluation = 0;
//     if (piece == PAWN || piece == pawn) p_evaluation += 100;
//     if (piece == KNIGHT || piece == knight || piece == BISHOP || piece == bishop) p_evaluation += 300;
//     if (piece == ROOK || piece == rook) p_evaluation += 500;
//     if (piece == QUEEN || piece == queen) p_evaluation += 900;
//     if (piece == KING || piece == king) p_evaluation += 9000;
//     // if piece white, positive evaluation, else negative
//     return ((PAWN <= piece && piece <= KING) ? p_evaluation : -p_evaluation);
// }

long Board::perft_search(int depth) {

    long res = 0;
    if (depth == 0) return 1;
    MoveList moves = get_all_legal_moves();

    for (const BitMove &mv : moves) {

        BoardState savedState(*this);

        // { // OPTI: evaluation function slows this down :( by 1.5x !! need to optimize
        //     int evaluation = 0;
        //     for (int i = 0; i < 8; i++)
        //         for (int j = 0; j < 8; j++)
        //             evaluation += evaluate_piece(get_piece_on_square(BitOps::rf_to_square(i, j)));
        //     if (is_attacked(find_king(W), B)) {
        //         evaluation -= 30;
        //         BitMoveVec moves = get_all_legal_moves();
        //         if (moves.size() == 0) return -99999;
        //     } else if (is_attacked(find_king(B), W)) {  // if white checks black
        //         evaluation += 30;
        //         BitMoveVec moves = get_all_legal_moves();
        //         if (moves.size() == 0) return 99999;
        //     }
        // }

        if (make_move(mv) == -1) continue;

        /*
            std::cout << mv.get_piece() << std::endl;
            std::cout << pieceOnSquare[mv.get_to()] << std::endl << "--" << std::endl;
            std::cout << mv.get_from() << " " << mv.get_to()  << std::endl;
            std::cout << *this << std::endl;
        */
        // assert(pieceOnSquare[mv.get_to()] == mv.get_piece());

        res += perft_search(depth - 1);

        savedState.reapply(*this);
    }
    return res;
}

std::string Board::getFEN() {
    std::string fen;

    // for each square
    for (int rank = 0; rank < 8; ++rank) {
        int emptySquares = 0;
        for (int file = 0; file < 8; ++file) {
            int square = rf_to_square(rank, file);
            int piece = get_piece_on_square(square);
            // we get the piece

            if (piece == -1) {
                ++emptySquares;  // counter for empty squares
            } else {
                if (emptySquares > 0) {
                    fen += std::to_string(emptySquares);
                    emptySquares = 0;
                }
                fen += letter_pieces[get_color_piece(piece, piece >= PAWN && piece <= KING ? W : B)];
            }
        }
        if (emptySquares > 0) fen += std::to_string(emptySquares);
        if (rank < 7) fen += "/";
    }

    // turn
    fen += (turn == W ? " w " : " b ");
    // castling
    fen += (castlingRights == 0
                ? "-"
                : (std::string(castlingRights & WK ? "K" : "") + std::string(castlingRights & WQ ? "Q" : "")
                   + std::string(castlingRights & BK ? "k" : "") + std::string(castlingRights & BQ ? "q" : "")));

    std::cout << "the actual fen is : " << fen << std::endl;
    return fen;
}
