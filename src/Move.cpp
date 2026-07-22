#include "../include/Move.h"

Move::Move(uint8_t from, uint8_t to, MoveType moveType,
           Piece promotionPiece)
    : from(from),
      to(to),
      moveType(moveType),
      promotionPiece(promotionPiece)
{
}

uint8_t Move ::getFrom() const
{
    return from;
}
uint8_t Move ::getTo() const
{
    return to;
}
MoveType Move ::getMoveType() const
{
    return moveType;
}

Piece Move::getPromotionPiece() const
{
    return promotionPiece;
}
