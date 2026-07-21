#include "../include/AttackTables.h"
#include <iostream>

namespace AttackTables
{
    uint64_t knightAttacks[64];
    uint64_t kingAttacks[64];

    void initializeKnightAttacks()
    {
        // Knight Attack Table
        int rankOffsets
            [] = {2, 2, -2, -2, 1, 1, -1, -1};
        int fileOffsets[] = {1, -1, 1, -1, 2, -2, 2, -2};
        for (int square = 0; square < 64; square++)
        {
            knightAttacks[square] = 0ULL;
            int rank = (square / 8);
            int file = square % 8;

            for (int i = 0; i < 8; i++)
            {
                int newRank = rank + rankOffsets[i];
                int newFile = file + fileOffsets[i];

                if (newRank >= 0 && newFile >= 0 && newRank < 8 && newFile < 8)
                {
                    int newSquare = newRank * 8 + newFile;
                    knightAttacks[square] |= (1ULL << newSquare);
                }
            }
        }
    }

    void initializeKingAttacks()
    {
        // King Attack Table

        int kingRankOffsets
            [] = {1, 1, 1, -1, -1, -1, 0, 0};
        int kingFileOffsets[] = {-1, 0, 1, -1, 0, 1, -1, 1};

        for (int square = 0; square < 64; square++)
        {
            kingAttacks[square] = 0ULL;
            int rank = (square / 8);
            int file = square % 8;

            for (int i = 0; i < 8; i++)
            {
                int newRank = rank + kingRankOffsets[i];
                int newFile = file + kingFileOffsets[i];

                if (newRank >= 0 && newFile >= 0 && newRank < 8 && newFile < 8)
                {
                    int newSquare = newRank * 8 + newFile;
                    kingAttacks[square] |= (1ULL << newSquare);
                }
            }
        }
    }

    void initialize()
    {
        initializeKnightAttacks();
        initializeKingAttacks();
        // initializePawnAttacks();
    }

    void print(uint64_t bitboard)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int value = 8 * (7 - i) + j;
                if (bitboard & (1ULL << value))
                {
                    std::cout << "O" << " ";
                }
                else
                {

                    std::cout << "." << " ";
                }
            }
            std::cout << std::endl;
        }
    }
}