typedef unsigned long long U64;

/* source : https://wiki.sharewiz.net/doku.php?id=chess:programming:bitboards
 RANKS:
   8 |       56   57   58   59   60   61   62   63  (MSB,
   7 |       48   49   50   51   52   53   54   55  left)  
   6 |       40   41   42   43   44   45   46   47  
   5 |       32   33   34   35   36   37   38   39 
   4 |       24   25   26   27   28   29   30   31  
   3 |       16   17   18   19   20   21   22   23  
   2 |        8    9   10   11   12   13   14   15
   1 | (LSB,  0    1    2    3    4    5    6    7
       right)
           -------------------------------------------
 FILES:      a     b    c    d    e    f    g    h    
 */

enum Positions {
    a8,b8,c8,d8,e8,f8,g8,h8,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a1,b1,c1,d1,e1,f1,g1,h1,
};

struct bitboard_t {
    U64 whiteKing;
    U64 whiteQueens;
    U64 whiteRooks;
    U64 whiteBishops;
    U64 whiteKnights;
    U64 whitePawns;

    U64 blackKing;
    U64 blackQueens;
    U64 blackRooks;
    U64 blackBishops;
    U64 blackKnights;
    U64 blackPawns;

    U64 whitePieces; // whiteKing | whiteQueens | whiteRooks | whiteBishops | whiteKnights | whitePawns;
    U64 blackPieces; // blackKing | blackQueens | blackRooks | blackBishops | blackKnights | blackPawns;

    bitboard_t();
};

bitboard_t::bitboard_t() : whiteKing(d1), whiteQueens(0), whiteRooks(0), whiteBishops(0), whiteKnights(0), whitePawns(0),
          blackKing(0), blackQueens(0), blackRooks(0), blackBishops(0), blackKnights(0), blackPawns(0),
          whitePieces(0), blackPieces(0) {}
