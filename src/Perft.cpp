#include "Perft.h"
#include <iostream>
uint64_t Perft::perft(Board board, int depth)
{
    if (depth == 0)
    {
        return 1ULL;
    }
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);

    uint64_t nodes = 0;

    for (const Move &move : moves)
    {
        Board tempBoard = board;
        tempBoard.makeMove(move);

        nodes += perft(tempBoard, depth - 1);
    }

    return nodes;
}

void Perft::divide(Board board, int depth)
{
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);

    uint64_t nodes = 0;
    for (const Move &move : moves)
    {
        Board tempBoard = board;
        tempBoard.makeMove(move);
        uint64_t currNode = perft(tempBoard, depth - 1);
        nodes += currNode;
        std::cout << move.getFrom() << " " << move.getTo() << " -> " << currNode << std::endl;
    }
    std::cout << nodes << std::endl;
}