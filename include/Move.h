#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include "Piece.h"

enum class MoveType
{
    Quiet,
    Capture,
    KingCastle,
    QueenCastle,
    EnPassant,
    Promotion,
    PromotionCapture
};

class Move
{
private:
    uint8_t from;
    uint8_t to;
    MoveType moveType;
    Piece promotionPiece;

public:
    Move(uint8_t from, uint8_t to, MoveType moveType,
         Piece promotionPiece = Piece::None);
    uint8_t getFrom() const;
    uint8_t getTo() const;
    MoveType getMoveType() const;
};

#endif