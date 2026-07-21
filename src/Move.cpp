#include "../include/Move.h"

Move::Move(uint8_t from, uint8_t to, MoveType moveType,
           Piece promotionPiece)
    : from(from),
      to(to),
      moveType(moveType),
      promotionPiece(promotionPiece)
{
}