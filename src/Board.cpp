#include "../include/Board.h"
#include "../include/AttackTables.h"
#include <iostream>
#include <map>

void Board::initialize()
{
    pieces[static_cast<int>(Piece::WhitePawn)] = ((1ULL << 8) - 1ULL) << 8;
    pieces[static_cast<int>(Piece::WhiteKnight)] = (1ULL << 6) | (1ULL << 1);
    pieces[static_cast<int>(Piece::WhiteBishop)] = (1ULL << 5) | (1ULL << 2);
    pieces[static_cast<int>(Piece ::WhiteRook)] = (1ULL << 7) | (1ULL << 0);
    pieces[static_cast<int>(Piece::WhiteQueen)] = (1ULL << 3);
    pieces[static_cast<int>(Piece::WhiteKing)] = (1ULL << 4);

    pieces[static_cast<int>(Piece::BlackPawn)] = ((1ULL << 8) - 1ULL) << 48;
    pieces[static_cast<int>(Piece::BlackKnight)] = (1ULL << 57) | (1ULL << 62);
    pieces[static_cast<int>(Piece::BlackBishop)] = (1ULL << 58) | (1ULL << 61);
    pieces[static_cast<int>(Piece::BlackRook)] = (1ULL << 56) | (1ULL << 63);
    pieces[static_cast<int>(Piece::BlackQueen)] = (1ULL << 59);
    pieces[static_cast<int>(Piece::BlackKing)] = (1ULL << 60);

    whiteToMove = true;

    whiteCastleKingSide = true;
    whiteCastleQueenSide = true;
    blackCastleKingSide = true;
    blackCastleQueenSide = true;

    enPassantSquare = -1;

    halfMoveClock = 0;

    fullMoveNumber = 1;
}

void Board::print()
{

    std::map<int, char> mp;

    mp[0] = 'P';
    mp[1] = 'N';
    mp[2] = 'B';
    mp[3] = 'R';
    mp[4] = 'Q';
    mp[5] = 'K';
    mp[6] = 'p';
    mp[7] = 'n';
    mp[8] = 'b';
    mp[9] = 'r';
    mp[10] = 'q';
    mp[11] = 'k';

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int value = 8 * (7 - i) + j;
            bool printed = false;
            for (int k = 0; k < 12; k++)
            {
                if (pieces[k] & (1ULL << value))
                {
                    std::cout << mp[k] << " ";
                    printed = true;
                    break;
                }
            }
            if (printed == false)
            {
                std::cout << "." << " ";
            }
        }
        std::cout << std::endl;
    }
}

uint64_t Board::getWhiteOccupancy() const
{
    uint64_t board = 0ULL;
    for (int i = 0; i < 6; i++)
    {
        board |= pieces[i];
    }
    return board;
}
uint64_t Board::getBlackOccupancy() const
{
    uint64_t board = 0ULL;
    for (int i = 6; i < 12; i++)
    {
        board |= pieces[i];
    }
    return board;
}
uint64_t Board::getOccupied() const
{
    return (getBlackOccupancy() | getWhiteOccupancy());
}

bool Board::isWhiteToMove() const
{
    return whiteToMove;
}

uint64_t Board::getPieceBitboard(Piece piece) const
{
    return pieces[static_cast<int>(piece)];
}

int Board::getEnPassantSquare() const
{
    return enPassantSquare;
}

bool Board::isSquareAttacked(int square, bool byWhite) const
{
    // 1. Get the occupancy
    uint64_t occupied = getOccupied();

    // 2. Get all attacking pieces of the specified color
    uint64_t pawns;
    uint64_t knights;
    uint64_t bishops;
    uint64_t rooks;
    uint64_t queens;
    uint64_t king;

    if (byWhite)
    {
        pawns = pieces[static_cast<int>(Piece::WhitePawn)];
        knights = pieces[static_cast<int>(Piece::WhiteKnight)];
        bishops = pieces[static_cast<int>(Piece::WhiteBishop)];
        rooks = pieces[static_cast<int>(Piece ::WhiteRook)];
        queens = pieces[static_cast<int>(Piece::WhiteQueen)];
        king = pieces[static_cast<int>(Piece::WhiteKing)];
    }
    else
    {
        pawns = pieces[static_cast<int>(Piece::BlackPawn)];
        knights = pieces[static_cast<int>(Piece::BlackKnight)];
        bishops = pieces[static_cast<int>(Piece::BlackBishop)];
        rooks = pieces[static_cast<int>(Piece ::BlackRook)];
        queens = pieces[static_cast<int>(Piece::BlackQueen)];
        king = pieces[static_cast<int>(Piece::BlackKing)];
    }

    // 3. Pawn attacks
    while (pawns)
    {
        int pawnSquare = __builtin_ctzll(pawns);
        uint64_t attacks = byWhite ? AttackTables::whitePawnAttacks[pawnSquare] : AttackTables::blackPawnAttacks[pawnSquare];
        if (attacks & (1ULL << square))
        {
            return true;
        }
        pawns &= pawns - 1;
    }
    // 4. Knight attacks

    while (knights)
    {
        int knightSquare = __builtin_ctzll(knights);
        uint64_t attacks = AttackTables::knightAttacks[knightSquare];
        if (attacks & (1ULL << square))
        {
            return true;
        }
        knights &= knights - 1;
    }

    // 5. Bishop attacks

    while (bishops)
    {
        int bishopSquare = __builtin_ctzll(bishops);
        uint64_t attacks = AttackTables::getBishopAttacks(bishopSquare, occupied);
        if (attacks & (1ULL << square))
        {
            return true;
        }
        bishops &= bishops - 1;
    }

    // 6. Rook attacks

    while (rooks)
    {
        int rookSquare = __builtin_ctzll(rooks);
        uint64_t attacks = AttackTables::getRookAttacks(rookSquare, occupied);
        if (attacks & (1ULL << square))
        {
            return true;
        }
        rooks &= rooks - 1;
    }

    // 7. Queen attacks

    while (queens)
    {
        int queenSquare = __builtin_ctzll(queens);
        uint64_t attacks = AttackTables::getQueenAttacks(queenSquare, occupied);
        if (attacks & (1ULL << square))
        {
            return true;
        }
        queens &= queens - 1;
    }

    // 8. King attacks

    while (king)
    {
        int kingSquare = __builtin_ctzll(king);
        uint64_t attacks = AttackTables::kingAttacks[kingSquare];
        if (attacks & (1ULL << square))
        {
            return true;
        }
        king &= king - 1;
    }
    return false;
}

bool Board::isWhiteCastleKingSide() const
{
    return whiteCastleKingSide;
}
bool Board::isWhiteCastleQueenSide() const
{
    return whiteCastleQueenSide;
}
bool Board::isBlackCastleKingSide() const
{
    return blackCastleKingSide;
}
bool Board::isBlackCastleQueenSide() const
{
    return blackCastleQueenSide;
}