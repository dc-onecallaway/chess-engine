#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include "Piece.h"
#include "Move.h"

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
    uint64_t getWhiteOccupancy() const;
    uint64_t getBlackOccupancy() const;
    uint64_t getOccupied() const;
    bool isWhiteToMove() const;
    uint64_t getPieceBitboard(Piece piece) const;
    int getEnPassantSquare() const;
    bool isSquareAttacked(int square, bool byWhite) const;

    bool isWhiteCastleKingSide() const;
    bool isWhiteCastleQueenSide() const;
    bool isBlackCastleKingSide() const;
    bool isBlackCastleQueenSide() const;

    void setWhiteCastleKingSide(bool value);
    void setWhiteCastleQueenSide(bool value);
    void setBlackCastleKingSide(bool value);
    void setBlackCastleQueenSide(bool value);

    void setEnPassantSquare(int square);

    void removePiece(Piece piece, int square);
    void addPiece(Piece piece, int square);
    Piece getPieceOnSquare(int square) const;
    void makeMove(const Move &move);
    int getKingSquare(bool white) const;
};

#endif