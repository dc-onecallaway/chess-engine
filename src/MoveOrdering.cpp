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

void MoveOrdering::orderMoves(Board &board, std::vector<Move> &moves, int ply, const Move &hashMove,
                              const Move &killer1,
                              const Move &killer2)
{
    std::sort(
        moves.begin(),
        moves.end(),
        [&](const Move &a, const Move &b)
        {
            return scoreMove(board, a, ply, hashMove, killer1, killer2) >
                   scoreMove(board, b, ply, hashMove, killer1, killer2);
        });
}

int MoveOrdering::scoreMove(Board &board, const Move &move, int ply, const Move &hashMove,
                            const Move &killer1,
                            const Move &killer2)
{
    MoveType type = move.getMoveType();

    if (move == hashMove)
        return 100000;

    if (move == killer1)
        return 60000;

    if (move == killer2)
        return 50000;

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