#include "../include/Search.h"
#include "../include/MoveGenerator.h"
#include "../include/Evaluation.h"
#include "../include/Move.h"
#include <iostream>

static long long nodes = 0;

Move Search::findBestMove(Board &board, int depth)
{
    // TODO
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);
    if (moves.empty())
    {
        // Handle checkmate/stalemate however you prefer
    }
    Move bestMove = moves[0];
    int maxScore = board.isWhiteToMove() ? INT_MIN : INT_MAX;

    for (const Move &move : moves)
    {
        board.makeMove(move);
        int score = minimax(board, depth - 1, INT_MIN, INT_MAX);
        board.undoMove();
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
    std::cout << "Nodes: " << nodes << '\n';
    nodes = 0;
    return bestMove;
}

int Search::minimax(Board &board, int depth, int alpha, int beta)
{
    nodes++;
    if (depth == 0)
        return quiescence(board, alpha, beta);
    ;
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);

    if (moves.empty())
    {
        constexpr int MATE_SCORE = 100000;
        if (board.isWhiteToMove())
        {
            if (board.isSquareAttacked(board.getKingSquare(true), false))
            {
                return -MATE_SCORE - depth;
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
                return MATE_SCORE + depth;
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

            board.makeMove(move);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove();
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
            if (alpha >= beta)
            {
                break;
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = INT_MAX;
        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove();
            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
            if (alpha >= beta)
            {
                break;
            }
        }
        return bestScore;
    }
}

int Search::quiescence(Board &board, int alpha, int beta)
{
    int standPat = Evaluation::evaluate(board);

    if (board.isWhiteToMove())
    {
        if (standPat >= beta)
        {
            return beta;
        }
        alpha = std::max(alpha, standPat);
        MoveGenerator generator;

        auto moves = generator.generateCaptureMoves(board);

        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = quiescence(board, alpha, beta);
            board.undoMove();
            alpha = std::max(alpha, score);
            if (alpha >= beta)
                break;
        }
        return alpha;
    }
    else
    {
        if (standPat <= alpha)
        {
            return alpha;
        }
        beta = std::min(beta, standPat);
        MoveGenerator generator;

        auto moves = generator.generateCaptureMoves(board);

        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = quiescence(board, alpha, beta);
            board.undoMove();
            beta = std::min(beta, score);
            if (alpha >= beta)
                break;
        }
        return beta;
    }
}