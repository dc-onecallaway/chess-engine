#include "../include/MoveOrdering.h"
#include <algorithm>

constexpr int PieceValues[] =
    {
        100, // White Pawn
        320,
        330,
        500,
        900,
        20000,

        100, // Black Pawn
        320,
        330,
        500,
        900,
        20000,

        0 // None
};

void MoveOrdering::orderMoves(Board &board, std::vector<Move> &moves)
{
    std::sort(
        moves.begin(),
        moves.end(),
        [&](const Move &a, const Move &b)
        {
            return scoreMove(board, a) >
                   scoreMove(board, b);
        });
}

int MoveOrdering::scoreMove(Board &board, const Move &move)
{
    MoveType type = move.getMoveType();

    if (type == MoveType::Promotion)
    {
        return 8000;
    }

    if (type == MoveType::Capture ||
        type == MoveType::PromotionCapture)
    {
        Piece attacker = board.getPieceOnSquare(move.getFrom());
        Piece victim = board.getPieceOnSquare(move.getTo());
        int attackerValue = PieceValues[static_cast<int>(attacker)];
        int victimValue = PieceValues[static_cast<int>(victim)];

        return 10 * victimValue - attackerValue;
    }

    if (type == MoveType::EnPassant)
    {
        Piece attacker = Piece::WhitePawn;
        return 10 * PieceValues[0] - PieceValues[0];
    }

    return 0;
}