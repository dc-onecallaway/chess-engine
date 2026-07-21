#include "../include/MoveGenerator.h"
#include "../include/AttackTables.h"

void MoveGenerator::generateKnightMoves(const Board &board, std::vector<Move> &moves)
{

    if (board.isWhiteToMove())
    {
        uint64_t knights = board.getPieceBitboard(Piece::WhiteKnight);

        while (knights)
        {
            int square = __builtin_ctzll(knights);
            uint64_t attacks = AttackTables::knightAttacks[square];
            uint64_t occupied = board.getWhiteOccupancy();
            uint64_t pseudoAttacks = (attacks & ~occupied);

            while (pseudoAttacks)
            {
                int attackSquare = __builtin_ctzll(pseudoAttacks);

                MoveType type =
                    (board.getBlackOccupancy() & (1ULL << attackSquare))
                        ? MoveType::Capture
                        : MoveType::Quiet;

                moves.emplace_back(square, attackSquare, type);
                pseudoAttacks &= (pseudoAttacks - 1);
            }

            knights &= (knights - 1);
        }
    }
    else
    {
        uint64_t knights = board.getPieceBitboard(Piece::BlackKnight);

        while (knights)
        {
            int square = __builtin_ctzll(knights);
            uint64_t attacks = AttackTables::knightAttacks[square];
            uint64_t occupied = board.getBlackOccupancy();
            uint64_t pseudoAttacks = (attacks & ~occupied);

            while (pseudoAttacks)
            {
                int attackSquare = __builtin_ctzll(pseudoAttacks);

                MoveType type =
                    (board.getWhiteOccupancy() & (1ULL << attackSquare))
                        ? MoveType::Capture
                        : MoveType::Quiet;

                moves.emplace_back(square, attackSquare, type);
                pseudoAttacks &= (pseudoAttacks - 1);
            }

            knights &= (knights - 1);
        }
    }
}