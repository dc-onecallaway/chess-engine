#pragma once

#include "Board.h"
#include "Move.h"
#include <vector>

class MoveOrdering
{
public:
    static void orderMoves(Board &board, std::vector<Move> &moves);

private:
    static int scoreMove(Board &board, const Move &move);
};