#include "../include/Search.h"
#include "../include/MoveGenerator.h"
#include "../include/Evaluation.h"
#include "../include/Move.h"

Move Search::findBestMove(Board board, int depth)
{
    // TODO
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);
    Move bestMove;
    int maxScore = board.isWhiteToMove() ? INT_MIN : INT_MAX;

    for (const Move &move : moves)
    {
        Board tempBoard = board;
        tempBoard.makeMove(move);
        int score = minimax(tempBoard, depth - 1);
        if (board.isWhiteToMove())
        {
            if (score > maxScore)
            {
                bestMove = move;
                maxScore = score;
            }
        }
        else
        {
            if (score < maxScore)
            {
                bestMove = move;
                maxScore = score;
            }
        }
    }
    return bestMove;
}

int Search::minimax(Board board, int depth)
{
    if (depth == 0)
        return Evaluation::evaluate(board);
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);

    if (moves.empty())
    {
        const int MATE_SCORE = 100000;
        if (board.isWhiteToMove())
        {
            if (board.isSquareAttacked(board.getKingSquare(true), false))
            {
                return -MATE_SCORE;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if (board.isSquareAttacked(board.getKingSquare(false), true))
            {
                return MATE_SCORE;
            }
            else
            {
                return 0;
            }
        }
    }

    if (board.isWhiteToMove())
    {
        int bestScore = INT_MIN;
        for (const Move &move : moves)
        {
            Board tempBoard = board;
            tempBoard.makeMove(move);

            int score = minimax(tempBoard, depth - 1);
            bestScore = std::max(bestScore, score);
        }
        return bestScore;
    }
    else
    {
        int bestScore = INT_MAX;
        for (const Move &move : moves)
        {
            Board tempBoard = board;
            tempBoard.makeMove(move);

            int score = minimax(tempBoard, depth - 1);
            bestScore = std::min(bestScore, score);
        }
        return bestScore;
    }
}