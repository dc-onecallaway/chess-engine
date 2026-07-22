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
    uint64_t occupied = ownOccupied | enemyOccupied;

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

    if (whiteToMove)
    {
        if (square == 4)
        {
            if (board.isWhiteCastleKingSide() &&
                !board.isSquareAttacked(5, false) &&
                !board.isSquareAttacked(6, false) &&
                !board.isSquareAttacked(4, false) &&
                !((1ULL << 5) & occupied) &&
                !((1ULL << 6) & occupied) &&
                ((1ULL << 7) & board.getPieceBitboard(Piece::WhiteRook)))
            {
                moves.emplace_back(square, 6, MoveType::KingCastle);
            }
            if (board.isWhiteCastleQueenSide() &&
                !board.isSquareAttacked(3, false) &&
                !board.isSquareAttacked(2, false) &&
                !board.isSquareAttacked(4, false) &&
                !((1ULL << 3) & occupied) &&
                !((1ULL << 2) & occupied) &&
                !((1ULL << 1) & occupied) &&
                ((1ULL << 0) & board.getPieceBitboard(Piece::WhiteRook)))
            {
                moves.emplace_back(square, 2, MoveType::QueenCastle);
            }
        }
    }
    else
    {
        if (square == 60)
        {
            if (board.isBlackCastleKingSide() &&
                !board.isSquareAttacked(61, true) &&
                !board.isSquareAttacked(62, true) &&
                !board.isSquareAttacked(60, true) &&
                !((1ULL << 61) & occupied) &&
                !((1ULL << 62) & occupied) &&
                ((1ULL << 63) & board.getPieceBitboard(Piece::BlackRook)))
            {
                moves.emplace_back(square, 62, MoveType::KingCastle);
            }
            if (board.isBlackCastleQueenSide() &&
                !board.isSquareAttacked(59, true) &&
                !board.isSquareAttacked(58, true) &&
                !board.isSquareAttacked(60, true) &&
                !((1ULL << 59) & occupied) &&
                !((1ULL << 58) & occupied) &&
                !((1ULL << 57) & occupied) &&
                ((1ULL << 56) & board.getPieceBitboard(Piece::BlackRook)))
            {
                moves.emplace_back(square, 58, MoveType::QueenCastle);
            }
        }
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

        int enPassantSquare = board.getEnPassantSquare();
        if (enPassantSquare != -1)
        {
            if (whiteToMove)
            {
                if (((1ULL) << enPassantSquare) & AttackTables::whitePawnAttacks[square])
                {
                    moves.emplace_back(square, enPassantSquare, MoveType::EnPassant);
                }
            }
            else
            {
                if (((1ULL) << enPassantSquare) & AttackTables::blackPawnAttacks[square])
                {
                    moves.emplace_back(square, enPassantSquare, MoveType::EnPassant);
                }
            }
        }

        pawn &= (pawn - 1);
    }
}

void MoveGenerator::generateBishopMoves(const Board &board, std::vector<Move> &moves)
{
    bool whiteToMove = board.isWhiteToMove();
    Piece bishop = whiteToMove ? Piece::WhiteBishop : Piece::BlackBishop;

    uint64_t ownOccupied = whiteToMove ? board.getWhiteOccupancy() : board.getBlackOccupancy();
    uint64_t enemyOccupied = whiteToMove ? board.getBlackOccupancy() : board.getWhiteOccupancy();
    uint64_t occupied = board.getOccupied();
    uint64_t bishops = board.getPieceBitboard(bishop);

    while (bishops)
    {
        int square = __builtin_ctzll(bishops);
        uint64_t attacks = AttackTables::getBishopAttacks(square, occupied);
        attacks &= ~ownOccupied;

        while (attacks)
        {
            int attackSquare = __builtin_ctzll(attacks);
            if (enemyOccupied & (1ULL << attackSquare))
            {
                moves.emplace_back(square, attackSquare, MoveType::Capture);
            }
            else
            {
                moves.emplace_back(square, attackSquare, MoveType::Quiet);
            }
            attacks &= (attacks - 1);
        }

        bishops &= (bishops - 1);
    }
}

void MoveGenerator::generateRookMoves(const Board &board, std::vector<Move> &moves)
{
    bool whiteToMove = board.isWhiteToMove();
    Piece rook = whiteToMove ? Piece::WhiteRook : Piece::BlackRook;

    uint64_t ownOccupied = whiteToMove ? board.getWhiteOccupancy() : board.getBlackOccupancy();
    uint64_t enemyOccupied = whiteToMove ? board.getBlackOccupancy() : board.getWhiteOccupancy();
    uint64_t occupied = board.getOccupied();
    uint64_t rooks = board.getPieceBitboard(rook);

    while (rooks)
    {
        int square = __builtin_ctzll(rooks);
        uint64_t attacks = AttackTables::getRookAttacks(square, occupied);
        attacks &= ~ownOccupied;

        while (attacks)
        {
            int attackSquare = __builtin_ctzll(attacks);
            if (enemyOccupied & (1ULL << attackSquare))
            {
                moves.emplace_back(square, attackSquare, MoveType::Capture);
            }
            else
            {
                moves.emplace_back(square, attackSquare, MoveType::Quiet);
            }
            attacks &= (attacks - 1);
        }

        rooks &= (rooks - 1);
    }
}

void MoveGenerator::generateQueenMoves(const Board &board, std::vector<Move> &moves)
{
    bool whiteToMove = board.isWhiteToMove();
    Piece queen = whiteToMove ? Piece::WhiteQueen : Piece::BlackQueen;

    uint64_t ownOccupied = whiteToMove ? board.getWhiteOccupancy() : board.getBlackOccupancy();
    uint64_t enemyOccupied = whiteToMove ? board.getBlackOccupancy() : board.getWhiteOccupancy();
    uint64_t occupied = board.getOccupied();
    uint64_t queens = board.getPieceBitboard(queen);

    while (queens)
    {
        int square = __builtin_ctzll(queens);
        uint64_t attacks = AttackTables::getQueenAttacks(square, occupied);
        attacks &= ~ownOccupied;

        while (attacks)
        {
            int attackSquare = __builtin_ctzll(attacks);
            if (enemyOccupied & (1ULL << attackSquare))
            {
                moves.emplace_back(square, attackSquare, MoveType::Capture);
            }
            else
            {
                moves.emplace_back(square, attackSquare, MoveType::Quiet);
            }
            attacks &= (attacks - 1);
        }
        queens &= (queens - 1);
    }
}

std::vector<Move> MoveGenerator::generateLegalMoves(const Board &board)
{
    std::vector<Move> pseudoMoves;

    generatePawnMoves(board, pseudoMoves);
    generateKnightMoves(board, pseudoMoves);
    generateBishopMoves(board, pseudoMoves);
    generateRookMoves(board, pseudoMoves);
    generateQueenMoves(board, pseudoMoves);
    generateKingMoves(board, pseudoMoves);

    std::vector<Move> legalMoves;

    // Check every pseudo move
    for (const Move &move : pseudoMoves)
    {
        Board tempBoard = board;  // Copy current position
        tempBoard.makeMove(move); // Play the move

        // Find our king
        int king = tempBoard.getKingSquare(!tempBoard.isWhiteToMove());
        // Check if our king is attacked
        // If not attacked -> legalMoves.push_back(move);
        if (!tempBoard.isSquareAttacked(king, tempBoard.isWhiteToMove()))
        {
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}