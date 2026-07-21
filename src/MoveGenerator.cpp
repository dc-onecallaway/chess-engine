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

void MoveGenerator::generatePawnMoves(const Board &board, std::vector<Move> &moves)
{
    bool whiteToMove = board.isWhiteToMove();
    Piece pawnType = whiteToMove ? Piece::WhitePawn : Piece::BlackPawn;
    uint64_t enemyOccupied = whiteToMove ? board.getBlackOccupancy() : board.getWhiteOccupancy();
    uint64_t pawn = board.getPieceBitboard(pawnType);
    uint64_t occupied = board.getOccupied();

    while (pawn)
    {
        int square = __builtin_ctzll(pawn);

        // 1. Capture

        uint64_t attacks = whiteToMove ? AttackTables::whitePawnAttacks[square] : AttackTables::blackPawnAttacks[square];
        uint64_t pseudoAttacks = (attacks & enemyOccupied);

        while (pseudoAttacks)
        {
            int attackSquare = __builtin_ctzll(pseudoAttacks);
            if ((attackSquare / 8 == 7) || (attackSquare / 8 == 0))
            {
                if (whiteToMove)
                {
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::WhiteQueen);
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::WhiteBishop);
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::WhiteKnight);
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::WhiteRook);
                }
                else
                {
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::BlackQueen);
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::BlackBishop);
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::BlackKnight);
                    moves.emplace_back(square, attackSquare, MoveType::PromotionCapture, Piece::BlackRook);
                }
            }
            else
            {
                moves.emplace_back(square, attackSquare, MoveType::Capture);
            }
            pseudoAttacks &= (pseudoAttacks - 1);
        }

        // 2. Single push And Promotion

        int singlePush = (whiteToMove) ? (square + 8) : (square - 8);
        if (singlePush >= 0 && singlePush < 64 && ((1ULL << singlePush) & ~occupied))
        {
            if (((singlePush / 8) == 7 || (singlePush / 8) == 0))
            {
                if (whiteToMove)
                {
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::WhiteQueen);
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::WhiteBishop);
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::WhiteKnight);
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::WhiteRook);
                }
                else
                {
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::BlackQueen);
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::BlackBishop);
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::BlackKnight);
                    moves.emplace_back(square, singlePush, MoveType::Promotion, Piece::BlackRook);
                }
            }
            else
            {
                // Single Push
                moves.emplace_back(square, singlePush, MoveType::Quiet);
            }
        }

        // 3. Double push

        if (whiteToMove)
        {
            if ((square / 8) == 1)
            {
                int doublePush = square + 16;
                if (((1ULL << singlePush) & ~occupied) && (((1ULL << doublePush) & ~occupied)))
                {
                    moves.emplace_back(square, doublePush, MoveType::Quiet);
                }
            }
        }
        else
        {
            if ((square / 8) == 6)
            {
                int doublePush = square - 16;
                if (((1ULL << singlePush) & ~occupied) && (((1ULL << doublePush) & ~occupied)))
                {
                    moves.emplace_back(square, doublePush, MoveType::Quiet);
                }
            }
        }

        // 5. En passant

        pawn &= (pawn - 1);
    }
}