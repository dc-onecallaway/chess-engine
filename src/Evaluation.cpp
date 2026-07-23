#include "../include/Evaluation.h"

int Evaluation::evaluate(const Board &board)
{
    // TODO
    int score = 0;

    score += (__builtin_popcountll(board.getPieceBitboard(Piece::WhitePawn)) * 100);
    score -= (__builtin_popcountll(board.getPieceBitboard(Piece::BlackPawn)) * 100);
    score += (__builtin_popcountll(board.getPieceBitboard(Piece::WhiteBishop)) * 330);
    score -= (__builtin_popcountll(board.getPieceBitboard(Piece::BlackBishop)) * 330);
    score += (__builtin_popcountll(board.getPieceBitboard(Piece::WhiteKnight)) * 320);
    score -= (__builtin_popcountll(board.getPieceBitboard(Piece::BlackKnight)) * 320);
    score += (__builtin_popcountll(board.getPieceBitboard(Piece::WhiteRook)) * 500);
    score -= (__builtin_popcountll(board.getPieceBitboard(Piece::BlackRook)) * 500);
    score += (__builtin_popcountll(board.getPieceBitboard(Piece::WhiteQueen)) * 900);
    score -= (__builtin_popcountll(board.getPieceBitboard(Piece::BlackQueen)) * 900);

    return score;
}