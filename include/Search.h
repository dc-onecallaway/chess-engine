#ifndef SEARCH_H
#define SEARCH_H

#include "Board.h"
#include "Move.h"

class Search
{
public:
    static Move findBestMove(Board board, int depth);
    static int minimax(Board board, int depth, int alpha, int beta);
};

#endif