#include "../include/Search.h"
#include "../include/MoveGenerator.h"
#include "../include/Evaluation.h"
#include "../include/Move.h"
#include "../include/MoveOrdering.h"
#include "../include/TranspositionTable.h"
#include <iostream>

static long long nodes = 0;

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
                return -MATE_SCORE - depth; // checkmate
            else
                return 0; // stalemate
        }
        else
        {
            if (board.isSquareAttacked(board.getKingSquare(false), true))
                return MATE_SCORE + depth; // checkmate
            else
                return 0; // stalemate
        }
    }

    MoveOrdering::orderMoves(board, moves);

    // TT hash move ordering
    TTEntry entry;
    if (TranspositionTable::probe(board.getHashKey(), 0, entry))
    {
        for (size_t i = 0; i < moves.size(); i++)
        {
            if (moves[i] == entry.bestMove)
            {
                std::swap(moves[0], moves[i]);
                break;
            }
        }
    }

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
    Move hashMove;

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
    MoveOrdering::orderMoves(board, moves);

    if (hasHashMove)
    {
        bool matched = false;
        for (size_t i = 0; i < moves.size(); i++)
        {
            if (moves[i] == hashMove)
            {
                std::swap(moves[0], moves[i]);
                matched = true;
                break;
            }
        }
        static long long tries = 0, hits = 0;
        tries++;
        if (matched)
            hits++;
        if (tries % 100000 == 0)
            std::cerr << "hash move hit rate: " << hits << "/" << tries << "\n";
    }

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
                break;
            }
        }
        TTFlag flag;

        if (bestScore <= originalAlpha)
            flag = TTFlag::UpperBound;
        if (bestScore >= originalBeta)
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
        MoveOrdering::orderMoves(board, moves);

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
        MoveOrdering::orderMoves(board, moves);

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