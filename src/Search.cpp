#include "../include/Search.h"
#include "../include/MoveGenerator.h"
#include "../include/Evaluation.h"
#include "../include/Move.h"
#include "../include/MoveOrdering.h"
#include "../include/TranspositionTable.h"
#include <iostream>

static long long nodes = 0;

Move Search::findBestMove(Board &board, int depth)
{
    // TODO
    MoveGenerator generator;
    std::vector<Move> moves = generator.generateLegalMoves(board);
    MoveOrdering::orderMoves(board, moves);
    if (moves.empty())
    {
        // Handle checkmate/stalemate however you prefer
        return Move();
    }

    // Optional but recommended: use a TT hash move at the root too,
    // same as minimax does, for better move ordering.
    TTEntry rootEntry;
    if (TranspositionTable::probe(board.getHashKey(), rootEntry))
    {
        for (size_t i = 0; i < moves.size(); i++)
        {
            if (moves[i] == rootEntry.bestMove)
            {
                std::swap(moves[0], moves[i]);
                break;
            }
        }
    }

    Move bestMove = moves[0];
    bool whiteToMove = board.isWhiteToMove();

    int alpha = INT_MIN;
    int beta = INT_MAX;

    if (whiteToMove)
    {
        int bestScore = INT_MIN;
        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove();

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
            alpha = std::max(alpha, bestScore);
            // no beta cutoff needed at root (no sibling above to prune against)
        }
    }
    else
    {
        int bestScore = INT_MAX;
        for (const Move &move : moves)
        {
            board.makeMove(move);
            int score = minimax(board, depth - 1, alpha, beta);
            board.undoMove();

            if (score < bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
            beta = std::min(beta, bestScore);
        }
    }
    std::cout << "Nodes: " << nodes << '\n';
    nodes = 0;
    return bestMove;
}

int Search::minimax(Board &board, int depth, int alpha, int beta)
{
    nodes++;
    TTEntry entry;
    bool hasHashMove = false;
    Move hashMove;

    if (TranspositionTable::probe(board.getHashKey(), entry))
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
        return quiescence(board, alpha, beta);
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
        Move bestMove;
        for (const Move &move : moves)
        {

            board.makeMove(move);
            int score = minimax(board, depth - 1, alpha, beta);
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
        else if (bestScore >= beta)
            flag = TTFlag::LowerBound;
        else
            flag = TTFlag::Exact;

        TranspositionTable::store(
            board.getHashKey(),
            depth,
            bestScore,
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
            int score = minimax(board, depth - 1, alpha, beta);
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
            bestScore,
            flag, bestMove);

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
        MoveOrdering::orderMoves(board, moves);

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
        MoveOrdering::orderMoves(board, moves);

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