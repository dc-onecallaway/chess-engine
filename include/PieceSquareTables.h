#ifndef PIECE_SQUARE_TABLES_H
#define PIECE_SQUARE_TABLES_H

namespace PieceSquareTables
{
    extern const int PawnTable[64];
    extern const int KnightTable[64];
    extern const int BishopTable[64];
    extern const int RookTable[64];
    extern const int QueenTable[64];
    extern const int KingTable[64];
    inline int mirrorSquare(int square)
    {
        return square ^ 56;
    }
}

#endif