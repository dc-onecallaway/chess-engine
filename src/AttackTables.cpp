#include "../include/AttackTables.h"
#include <iostream>

namespace AttackTables
{
    uint64_t knightAttacks[64];

    void initialize()
    {
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

    void print(int square)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int value = 8 * (7 - i) + j;
                if (knightAttacks[square] & (1ULL << value))
                {
                    std::cout << "N" << " ";
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