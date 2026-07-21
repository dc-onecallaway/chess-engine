#include "../include/MoveGenerator.h"
#include "../include/AttackTables.h"

void MoveGenerator::generateKnightMoves(const Board &board, std::vector<Move> &moves)
{
    bool whiteToMove = board.isWhiteToMove();
    Piece knight = whiteToMove ? Piece::WhiteKnight : Piece::BlackKnight;

    uint64_t ownOccupied = whiteToMove ? board.getWhiteOccupancy() : board.getBlackOccupancy();
    uint64_t enemyOccupied = whiteToMove ? board.getBlackOccupancy() : board.getWhiteOccupancy();

    uint64_t knights = board.getPieceBitboard(knight);

    while (knights)
    {
        int square = __builtin_ctzll(knights);
        uint64_t attacks = AttackTables::knightAttacks[square];
        uint64_t pseudoAttacks = (attacks & ~ownOccupied);

        while (pseudoAttacks)
        {
            int attackSquare = __builtin_ctzll(pseudoAttacks);

            MoveType type =
                (enemyOccupied & (1ULL << attackSquare))
                    ? MoveType::Capture
                    : MoveType::Quiet;

            moves.emplace_back(square, attackSquare, type);
            pseudoAttacks &= (pseudoAttacks - 1);
        }

        knights &= (knights - 1);
    }
}

void MoveGenerator::generateKingMoves(const Board &board, std::vector<Move> &moves)
{
    bool whiteToMove = board.isWhiteToMove();
    Piece kingType = whiteToMove ? Piece::WhiteKing : Piece::BlackKing;

    uint64_t ownOccupied = whiteToMove ? board.getWhiteOccupancy() : board.getBlackOccupancy();
    uint64_t enemyOccupied = whiteToMove ? board.getBlackOccupancy() : board.getWhiteOccupancy();

    uint64_t king = board.getPieceBitboard(kingType);

    int square = __builtin_ctzll(king);
    uint64_t attacks = AttackTables::kingAttacks[square];
    uint64_t pseudoAttacks = (attacks & ~ownOccupied);

    while (pseudoAttacks)
    {
        int attackSquare = __builtin_ctzll(pseudoAttacks);

        MoveType type =
            (enemyOccupied & (1ULL << attackSquare))
                ? MoveType::Capture
                : MoveType::Quiet;

        moves.emplace_back(square, attackSquare, type);
        pseudoAttacks &= (pseudoAttacks - 1);
    }
}