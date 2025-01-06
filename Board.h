#pragma once
#include <array>
#include <cstring>

#include "BitMove.h"
#include "Definitions.h"

// TODO: replace with references where possible, should speed boost if understood correctly
class Board {
   public:
    /* --------------------------------- fields --------------------------------- */

    // one bitboard per piece per color, bit number x are set to 1 if piece is on square x (x from 0 to 63)
    // see enum Squares in Definitions.h to get an idea of the square numbering
    U64 bitboards[12];

    // one bitboard per color, bit number x are set to 1 if any piece of that color is on square x
    // colors are W, B, WB (white and black) (defined in Definitions.h)
    U64 occupancies[3];

    // enpassant square, -1 if no enpassant square
    int enpassantSquare;

    // castling rights (there are 4 types of castling), see Definitions.h for the bitmasks, 0 if no castling rights
    int castlingRights;

    // current turn, W or B
    int turn;

    // to speed up piece lookup
    std::array<int, 64> pieceOnSquare;

    /* ----------------------- constructor & board-related ---------------------- */

    // constructors
    // Board(bool initpieces = true);
    // initialize the board with a FEN string, useful for testing
    Board();
    Board(const std::string &fen);

    // properly clear the board (does not reset to initial position, just empties the board)
    void clear_board();

   private:
    // void setup_initial_pieces();
    void update_occupancies();
    void init_attacks();

   public:
    /* ---------------------------- helper functions ---------------------------- */

    // allows 'std::cout'-ing the board
    friend std::ostream &operator<<(std::ostream &os, const Board &board);

    // get the piece on a square, -1 if no piece
    // returned values are the piece codes defined in Definitions.h (enum Pieces)
    int get_piece_on_square(int square);

    // check if a square is empty
    bool is_empty(int square);

    // gets the correct piece code, with color adjusted to the given color, based on the piece codes of enum Pieces in
    // Definitions.h
    int get_color_piece(int piece, int color);

    // find the square of the king of given color
    int find_king(int color);

    /* ------------------------------ move parsing ------------------------------ */

    // makes a move on the board, handling special moves (captures, promotions, enpassant, castling, etc.) and updating
    // board state properly (turn, occupancies, etc.)
    // return -1 if move is illegal, 0 otherwise
    // set justCheckCheck to true to not alter board state and just check for checks
    int move(const BitMove &move, bool justCheckCheck = false);

    /* ----------------------------- attack-related ----------------------------- */

    // precalculated attack bitmasks, built in init_attacks() at instanciation, to save time later
    U64 pawnAttacks[2][64] = {0};
    U64 knightAttacks[64] = {0};
    U64 kingAttacks[64] = {0};

    // check if a square is attacked by any piece of given color
    bool is_attacked(int square, int by_color);

    // add a move to a move vector only if it is legal (i.e. does not lead to piece capture nor king in check)
    void add_move_if_legal(BitMoveVec &moveVec, const BitMove &m);

    // generate all legal moves for the current board configuration, using the bitpieces/* functions
    BitMoveVec get_all_legal_moves();

    // for debugging purposes, output to be compared with stockfish
    void perftree(int depth);
    long perft_search(int depth);
};

// object to store the state of the board, to allow for easy way of undoing moves
class BoardState {
   public:
    U64 bitboards[12];
    U64 occupancies[3];
    int turn;
    int enpassantSquare;
    int castlingRights;

    BoardState(const Board &board) {
        // std::copy(std::begin(board.bitboards), std::end(board.bitboards), std::begin(bitboards));
        // std::copy(std::begin(board.occupancies), std::end(board.occupancies), std::begin(occupancies));
        // static_assert(std::is_trivially_copyable_v<uint64_t>, "bitboards and occupancies must be trivially
        // copyable."); static_assert(std::is_trivially_copyable_v<int>, "Board metadata must be trivially copyable.");
        // memcpy significantly faster !
        // FIXME: error when compiling without -O
        // Program received signal SIGSEGV, Segmentation fault.
        // __memcpy_avx512_unaligned_erms ()
        //     at ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:273
        // 273     ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S: No such file or directory.
        // (gdb) bt
        // #0  __memcpy_avx512_unaligned_erms ()
        //     at ../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:273
        // #1  0x00005555555663dd in std::__copy_move<false, true, std::random_access_iterator_tag>::__copy_m<unsigned
        // long long> (__first=0x5fffffffd050,
        //     __last=0x5fffffffd0b0, __result=0x7fffffffc930)
        //     at /usr/include/c++/12/bits/stl_algobase.h:431
        // #2  0x0000555555566284 in std::__copy_move_a2<false, unsigned long long const*, unsigned long long*> (
        std::memcpy(bitboards, board.bitboards, sizeof(bitboards));
        std::memcpy(occupancies, board.occupancies, sizeof(occupancies));

        turn = board.turn;
        enpassantSquare = board.enpassantSquare;
        castlingRights = board.castlingRights;
    }

    void reapply(Board &board) const {
        // std::copy(std::begin(bitboards), std::end(bitboards), std::begin(board.bitboards));
        // std::copy(std::begin(occupancies), std::end(occupancies), std::begin(board.occupancies));
        // static_assert(std::is_trivially_copyable_v<uint64_t>, "bitboards and occupancies must be trivially
        // copyable."); static_assert(std::is_trivially_copyable_v<int>, "Board metadata must be trivially copyable.");
        // memcpy significantly faster !
        std::memcpy(board.bitboards, bitboards, sizeof(bitboards));
        std::memcpy(board.occupancies, occupancies, sizeof(occupancies));

        board.turn = turn;
        board.enpassantSquare = enpassantSquare;
        board.castlingRights = castlingRights;
    }
};