#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include "Piece.h"

class Board
{
private:
    uint64_t pieces[12];

    bool whiteToMove;

    bool whiteCastleKingSide;
    bool whiteCastleQueenSide;
    bool blackCastleKingSide;
    bool blackCastleQueenSide;

    int enPassantSquare;

    int halfMoveClock;

    int fullMoveNumber;

public:
    void initialize();
    void print();
};

#endif