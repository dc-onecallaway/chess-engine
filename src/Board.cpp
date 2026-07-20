#include "../include/Board.h"
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