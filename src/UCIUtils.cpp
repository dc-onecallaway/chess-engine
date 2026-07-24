#include "../include/UCIUtils.h"
#include "../include/MoveGenerator.h"

std::string UCIUtils::moveToString(const Move &move)
{
    int from = move.getFrom();
    int to = move.getTo();
    MoveType mType = move.getMoveType();
    std::string result = "";
    result += (char)(from % 8 + 'a');
    result += (char)(from / 8 + '1');
    result += (char)(to % 8 + 'a');
    result += (char)(to / 8 + '1');
    if (mType == MoveType::Promotion || mType == MoveType::PromotionCapture)
    {
        Piece piece = move.getPromotionPiece();
        if (piece == Piece::WhiteQueen || piece == Piece::BlackQueen)
        {
            result += 'q';
        }
        else if (piece == Piece::WhiteKnight || piece == Piece::BlackKnight)
        {
            result += 'n';
        }
        else if (piece == Piece::WhiteBishop || piece == Piece::BlackBishop)
        {
            result += 'b';
        }
        else
        {
            result += 'r';
        }
    }

    return result;
}

Move UCIUtils::parseMove(const Board &board, const std::string &moveString)
{
    MoveGenerator generator;
    auto legalMoves = generator.generateLegalMoves(board);

    for (const Move &move : legalMoves)
    {
        if (moveToString(move) == moveString)
            return move;
    }
    return Move();
}