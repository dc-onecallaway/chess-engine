#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include <vector>
#include "Board.h"
#include "Move.h"

class MoveGenerator
{
private:
public:
    void generateKnightMoves(const Board &board, std::vector<Move> &moves); // Pass by reference and no changes to board
};

#endif