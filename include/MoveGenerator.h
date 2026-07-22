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
    void generateKingMoves(const Board &board, std::vector<Move> &moves);
    void generatePawnMoves(const Board &board, std::vector<Move> &moves);
    void generateBishopMoves(const Board &board, std::vector<Move> &moves);
    void generateRookMoves(const Board &board, std::vector<Move> &moves);
    void generateQueenMoves(const Board &board, std::vector<Move> &moves);
    std::vector<Move> generateLegalMoves(const Board &board);
};

#endif