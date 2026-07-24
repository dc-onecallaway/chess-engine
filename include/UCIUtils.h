#ifndef UCIUTILS_H
#define UCIUTILS_H

#include "Board.h"
#include "Move.h"
#include <string>

namespace UCIUtils
{
    Move parseMove(const Board &board, const std::string &moveString);
    std::string moveToString(const Move &move);
}

#endif