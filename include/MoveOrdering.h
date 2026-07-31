#pragma once

#include "Board.h"
#include "Move.h"
#include <vector>

class MoveOrdering
{
public:
    static void orderMoves(Board &board, std::vector<Move> &moves, int ply, const Move &hashMove,
                           const Move &killer1,
                           const Move &killer2);

private:
    static int scoreMove(Board &board, const Move &move, int ply, const Move &hashMove,
                         const Move &killer1,
                         const Move &killer2);
};