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

    void generatePawnCaptures(const Board &board, std::vector<Move> &pseudoMoves);
    void generateKnightCaptures(const Board &board, std::vector<Move> &pseudoMoves);
    void generateBishopCaptures(const Board &board, std::vector<Move> &pseudoMoves);
    void generateRookCaptures(const Board &board, std::vector<Move> &pseudoMoves);
    void generateQueenCaptures(const Board &board, std::vector<Move> &pseudoMoves);
    void generateKingCaptures(const Board &board, std::vector<Move> &pseudoMoves);

    std::vector<Move> generateLegalMoves(Board &board);
    std::vector<Move> generateCaptureMoves(Board &board);
};

#endif