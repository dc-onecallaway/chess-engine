#include "../include/Evaluation.h"
#include "../include/PieceSquareTables.h"

int Evaluation::evaluate(const Board &board)
{
    // TODO
    int score = 0;

    uint64_t whitePawn = board.getPieceBitboard(Piece::WhitePawn);
    while (whitePawn)
    {
        int square = __builtin_ctzll(whitePawn);
        score += 100;
        score += PieceSquareTables::PawnTable[square];
        whitePawn &= (whitePawn - 1);
    }

    uint64_t blackPawn = board.getPieceBitboard(Piece::BlackPawn);
    while (blackPawn)
    {
        int square = __builtin_ctzll(blackPawn);
        score -= 100;
        score -= PieceSquareTables::PawnTable[PieceSquareTables::mirrorSquare(square)];
        blackPawn &= (blackPawn - 1);
    }

    uint64_t whiteKnight = board.getPieceBitboard(Piece::WhiteKnight);
    while (whiteKnight)
    {
        int square = __builtin_ctzll(whiteKnight);
        score += 320;
        score += PieceSquareTables::KnightTable[square];
        whiteKnight &= (whiteKnight - 1);
    }

    uint64_t blackKnight = board.getPieceBitboard(Piece::BlackKnight);
    while (blackKnight)
    {
        int square = __builtin_ctzll(blackKnight);
        score -= 320;
        score -= PieceSquareTables::KnightTable[PieceSquareTables::mirrorSquare(square)];
        blackKnight &= (blackKnight - 1);
    }

    uint64_t whiteBishop = board.getPieceBitboard(Piece::WhiteBishop);
    while (whiteBishop)
    {
        int square = __builtin_ctzll(whiteBishop);
        score += 330;
        score += PieceSquareTables::BishopTable[square];
        whiteBishop &= (whiteBishop - 1);
    }

    uint64_t blackBishop = board.getPieceBitboard(Piece::BlackBishop);
    while (blackBishop)
    {
        int square = __builtin_ctzll(blackBishop);
        score -= 330;
        score -= PieceSquareTables::BishopTable[PieceSquareTables::mirrorSquare(square)];
        blackBishop &= (blackBishop - 1);
    }
    uint64_t whiteRook = board.getPieceBitboard(Piece::WhiteRook);
    while (whiteRook)
    {
        int square = __builtin_ctzll(whiteRook);
        score += 500;
        score += PieceSquareTables::RookTable[square];
        whiteRook &= (whiteRook - 1);
    }

    uint64_t blackRook = board.getPieceBitboard(Piece::BlackRook);
    while (blackRook)
    {
        int square = __builtin_ctzll(blackRook);
        score -= 900;
        score -= PieceSquareTables::RookTable[PieceSquareTables::mirrorSquare(square)];
        blackRook &= (blackRook - 1);
    }
    uint64_t whiteQueen = board.getPieceBitboard(Piece::WhiteQueen);
    while (whiteQueen)
    {
        int square = __builtin_ctzll(whiteQueen);
        score += 900;
        score += PieceSquareTables::QueenTable[square];
        whiteQueen &= (whiteQueen - 1);
    }

    uint64_t blackQueen = board.getPieceBitboard(Piece::BlackQueen);
    while (blackQueen)
    {
        int square = __builtin_ctzll(blackQueen);
        score -= 500;
        score -= PieceSquareTables::QueenTable[PieceSquareTables::mirrorSquare(square)];
        blackQueen &= (blackQueen - 1);
    }
    score += PieceSquareTables::KingTable[board.getKingSquare(true)];
    score -= PieceSquareTables::KingTable[PieceSquareTables::mirrorSquare(board.getKingSquare(false))];
    return score;
}