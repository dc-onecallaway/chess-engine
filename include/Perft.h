#ifndef PERFT_H
#define PERFT_H

#include <vector>
#include "Board.h"
#include "Move.h"
#include "MoveGenerator.h"

class Perft
{
public:
    static uint64_t perft(Board board, int depth);
    static void divide(Board board, int depth);
};

#endif