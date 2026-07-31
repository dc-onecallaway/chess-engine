#include "../include/Search.h"
#include "../include/MoveGenerator.h"
#include "../include/Evaluation.h"
#include "../include/Move.h"
#include "../include/MoveOrdering.h"
#include "../include/TranspositionTable.h"
#include <iostream>

static long long nodes = 0;
Move Search::killerMoves[64][2];

int Search::searchRoot(Board &board, int depth, Move &bestMove)
{
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);

    if (moves.empty())
    {
        constexpr int MATE_SCORE = 100000;
        if (board.isWhiteToMove())
        {
            if (board.isSquareAttacked(board.getKingSquare(true), false))
                return -MATE_SCORE; // checkmate
            else
                return 0; // stalemate
        }
        else
        {
            if (board.isSquareAttacked(board.getKingSquare(false), true))
                return MATE_SCORE; // checkmate
            else
                return 0; // stalemate
        }
    }

    Move hashMove;
    TTEntry entry;

    if (TranspositionTable::probe(board.getHashKey(), 0, entry))
        hashMove = entry.bestMove;

    MoveOrdering::orderMoves(
        board,
        moves, 0,
        hashMove,
        killerMoves[0][0],
        killerMoves[0][1]);

    int alpha = INT_MIN;
    int beta = INT_MAX;
    int originalAlpha = alpha;
    int originalBeta = beta;

    bool maximizing = board.isWhiteToMove();

    int bestScore = maximizing ? INT_MIN : INT_MAX;
    bestMove = moves[0];

    for (const Move &move : moves)
    {
        board.makeMove(move);

        int score = minimax(board, depth - 1, alpha, beta, 1);

        board.undoMove();

        if (maximizing)
        {
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }

            alpha = std::max(alpha, bestScore);
        }
        else
        {
            if (score < bestScore)
            {
                bestScore = score;
                bestMove = move;
            }

            beta = std::min(beta, bestScore);
        }
    }

    // Store root result in TT, same classification scheme as minimax
    TTFlag flag;
    if (maximizing)
    {
        if (bestScore <= originalAlpha)
            flag = TTFlag::UpperBound;
        else if (bestScore >= beta)
            flag = TTFlag::LowerBound;
        else
            flag = TTFlag::Exact;
    }
    else
    {
        if (bestScore <= originalAlpha)
            flag = TTFlag::UpperBound;
        else if (bestScore >= originalBeta)
            flag = TTFlag::LowerBound;
        else
            flag = TTFlag::Exact;
    }

    TranspositionTable::store(
        board.getHashKey(),
        depth,
        bestScore, 0,
        flag, bestMove);

    return bestScore;
}

Move Search::findBestMove(Board &board, int maxDepth)
{
    for (int i = 0; i < 64; i++)
    {
        killerMoves[i][0] = Move();
        killerMoves[i][1] = Move();
    }
    Move bestMove;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
        nodes = 0;

        Move currentBest;

        auto start = std::chrono::steady_clock::now();

        int score = searchRoot(board, depth, currentBest);

        auto end = std::chrono::steady_clock::now();

        bestMove = currentBest;

        std::cout << "info depth "
                  << depth
                  << " score cp "
                  << score
                  << " nodes "
                  << nodes
                  << " time "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << std::endl;
    }

    return bestMove;
}

int Search::minimax(Board &board, int depth, int alpha, int beta, int ply)
{
    nodes++;
    TTEntry entry;
    bool hasHashMove = false;
    Move hashMove = Move();

    if (TranspositionTable::probe(board.getHashKey(), ply, entry))
    {
        hasHashMove = true;
        hashMove = entry.bestMove;
        if (entry.depth >= depth)
        {
            if (entry.flag == TTFlag::Exact)
                return entry.score;

            if (entry.flag == TTFlag::LowerBound)
                alpha = std::max(alpha, entry.score);

            if (entry.flag == TTFlag::UpperBound)
                beta = std::min(beta, entry.score);

            if (alpha >= beta)
                return entry.score;
        }
    }

    int originalAlpha = alpha;
    int originalBeta = beta;

    if (depth == 0)
        return quiescence(board, alpha, beta, ply);
    ;
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);
    MoveOrdering::orderMoves(
        board,
        moves, ply,
        hashMove,
        killerMoves[ply][0],
        killerMoves[ply][1]);

    if (moves.empty())
    {
        constexpr int MATE_SCORE = 100000;
        if (board.isWhiteToMove())
        {
            if (board.isSquareAttacked(board.getKingSquare(true), false))
            {
                return -MATE_SCORE + ply;
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
                return MATE_SCORE - ply;
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
        Move bestMove;
        for (const Move &move : moves)
        {

            board.makeMove(move);
            int score = minimax(board, depth - 1, alpha, beta, ply + 1);
            board.undoMove();

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }

            alpha = std::max(alpha, bestScore);
            if (alpha >= beta)
            {
                // Beta cutoff
                static long long totalCutoffs = 0, killerCutoffs = 0;
                totalCutoffs++;
                if (move == killerMoves[ply][0] || move == killerMoves[ply][1])
                    killerCutoffs++;
                if (totalCutoffs % 100000 == 0)
                    std::cerr << "killer cutoff rate: " << killerCutoffs << "/" << totalCutoffs;

                // Beta cutoff
                if (!((move.getMoveType() == MoveType::Capture) || (move.getMoveType() == MoveType::PromotionCapture)))
                {
                    if (!(killerMoves[ply][0] == move))
                    {
                        killerMoves[ply][1] = killerMoves[ply][0];
                        killerMoves[ply][0] = move;
                    }
                }
                break;
            }
        }
        TTFlag flag;

        if (bestScore <= originalAlpha)
            flag = TTFlag::UpperBound;
        else if (bestScore >= originalBeta)
            flag = TTFlag::LowerBound;
        else
            flag = TTFlag::Exact;

        TranspositionTable::store(
            board.getHashKey(),
            depth,
            bestScore, ply,
            flag, bestMove);

        return bestScore;
    }
    else
    {
        int bestScore = INT_MAX;
        Move bestMove;
        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = minimax(board, depth - 1, alpha, beta, ply + 1);
            board.undoMove();
            if (score < bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
            beta = std::min(beta, bestScore);
            if (alpha >= beta)
            {
                // Beta cutoff
                static long long totalCutoffs = 0, killerCutoffs = 0;
                totalCutoffs++;
                if (move == killerMoves[ply][0] || move == killerMoves[ply][1])
                    killerCutoffs++;
                if (totalCutoffs % 100000 == 0)
                    std::cerr << "killer cutoff rate: " << killerCutoffs << "/" << totalCutoffs;

                // Beta cutoff
                if (!((move.getMoveType() == MoveType::Capture) || (move.getMoveType() == MoveType::PromotionCapture)))
                {
                    if (!(killerMoves[ply][0] == move))
                    {
                        killerMoves[ply][1] = killerMoves[ply][0];
                        killerMoves[ply][0] = move;
                    }
                }
                break;
            }
        }
        TTFlag flag;

        if (bestScore <= originalAlpha)
            flag = TTFlag::UpperBound;
        else if (bestScore >= originalBeta)
            flag = TTFlag::LowerBound;
        else
            flag = TTFlag::Exact;

        TranspositionTable::store(
            board.getHashKey(),
            depth,
            bestScore, ply,
            flag, bestMove);

        return bestScore;
    }
}

int Search::quiescence(Board &board, int alpha, int beta, int ply)
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
        MoveOrdering::orderMoves(board, moves, ply, Move(), Move(), Move());

        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = quiescence(board, alpha, beta, ply + 1);
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
        MoveOrdering::orderMoves(board, moves, ply, Move(), Move(), Move());

        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = quiescence(board, alpha, beta, ply + 1);
            board.undoMove();
            beta = std::min(beta, score);
            if (alpha >= beta)
                break;
        }
        return beta;
    }
}