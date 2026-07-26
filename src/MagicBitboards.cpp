#include "../include/MagicBitboards.h"
#include <random>
#include <iostream>
#include <cstdint>

std::array<uint64_t, 64> MagicBitboards::rookMasks;
std::array<uint64_t, 64> MagicBitboards::bishopMasks;

std::array<int, 64> MagicBitboards::rookRelevantBits;
std::array<int, 64> MagicBitboards::bishopRelevantBits;

std::array<uint64_t, 64> MagicBitboards::rookMagics;
std::array<uint64_t, 64> MagicBitboards::bishopMagics;

std::array<std::vector<uint64_t>, 64> MagicBitboards::rookAttackTable;
std::array<std::vector<uint64_t>, 64> MagicBitboards::bishopAttackTable;

std::array<std::vector<uint64_t>, 64> MagicBitboards::rookAttacks;
std::array<std::vector<uint64_t>, 64> MagicBitboards::bishopAttacks;

std::array<std::vector<uint64_t>, 64> MagicBitboards::rookOccupancies;
std::array<std::vector<uint64_t>, 64> MagicBitboards::bishopOccupancies;

void MagicBitboards::initialize()
{
    initializeMasks();
    initializeAttackTables();
    buildMagicAttackTables();
}

void MagicBitboards::initializeMasks()
{
    // Generate rook masks
    for (int square = 0; square < 64; square++)
    {
        uint64_t rookMask = 0ULL;

        int rank = square >> 3;
        int file = square & 7;

        for (int i = 1; i < 7; i++)
        {
            rookMask |= (1ULL << ((rank << 3) + i));
            rookMask |= (1ULL << ((i << 3) + file));
        }
        rookMask &= ~(1ULL << square);
        rookMasks[square] = rookMask;
    }

    // Generate bishop masks
    for (int square = 0; square < 64; square++)
    {
        uint64_t bishopMask = 0ULL;

        int rank = square >> 3;
        int file = square & 7;

        // North-East
        for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++)
        {
            bishopMask |= 1ULL << ((r << 3) + f);
        }

        // North-West
        for (int r = rank + 1, f = file - 1; r <= 6 && f >= 1; r++, f--)
        {
            bishopMask |= 1ULL << ((r << 3) + f);
        }

        // South-East
        for (int r = rank - 1, f = file + 1; r >= 1 && f <= 6; r--, f++)
        {
            bishopMask |= 1ULL << ((r << 3) + f);
        }

        // South-West
        for (int r = rank - 1, f = file - 1; r >= 1 && f >= 1; r--, f--)
        {
            bishopMask |= 1ULL << ((r << 3) + f);
        }

        bishopMasks[square] = bishopMask;
    }
}

int MagicBitboards::countBits(uint64_t bb)
{
    return __builtin_popcountll(bb);
}

int MagicBitboards::getLSBIndex(uint64_t bb)
{
    return __builtin_ctzll(bb);
}

uint64_t MagicBitboards::setOccupancy(int index, int bitsInMask, uint64_t mask)
{
    uint64_t temp = mask;
    uint64_t occupancy = 0ULL;
    for (int i = 0; i < bitsInMask; i++)
    {
        int square = getLSBIndex(temp);
        if (index & (1 << i))
        {
            occupancy |= (1ULL << square);
        }
        temp &= (temp - 1);
    }
    return occupancy;
}

uint64_t MagicBitboards::generateRookAttack(int square, uint64_t blockers)
{
    uint64_t attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    const int rankOffSets[] = {0, 0, 1, -1};
    const int fileOffSets[] = {1, -1, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        int newRank = rank + rankOffSets[i];
        int newFile = file + fileOffSets[i];
        while (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8)
        {
            int newSquare = newRank * 8 + newFile;
            attacks |= (1ULL << newSquare);
            if (blockers & (1ULL << newSquare))
            {
                break;
            }
            newRank += rankOffSets[i];
            newFile += fileOffSets[i];
        }
    }

    return attacks;
}

uint64_t MagicBitboards::generateBishopAttack(int square, uint64_t blockers)
{
    uint64_t attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    const int rankOffSets[] = {-1, -1, 1, 1};
    const int fileOffSets[] = {1, -1, 1, -1};
    for (int i = 0; i < 4; i++)
    {
        int newRank = rank + rankOffSets[i];
        int newFile = file + fileOffSets[i];
        while (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8)
        {
            int newSquare = newRank * 8 + newFile;
            attacks |= (1ULL << newSquare);
            if (blockers & (1ULL << newSquare))
            {
                break;
            }
            newRank += rankOffSets[i];
            newFile += fileOffSets[i];
        }
    }
    return attacks;
}

void MagicBitboards::initializeAttackTables()
{

    for (int square = 0; square < 64; square++)
    {
        int rookBits = countBits(rookMasks[square]);
        rookRelevantBits[square] = rookBits;
        int rookOccupancyCount = 1 << rookBits;
        rookOccupancies[square].resize(rookOccupancyCount);
        rookAttacks[square].resize(rookOccupancyCount);

        for (int i = 0; i < rookOccupancyCount; i++)
        {
            uint64_t occ = setOccupancy(i, rookBits, rookMasks[square]);

            rookOccupancies[square][i] = occ;
            rookAttacks[square][i] = generateRookAttack(square, occ);
        }

        rookMagics[square] = findMagicNumber(square, rookBits, false);

        int bishopBits = countBits(bishopMasks[square]);
        bishopRelevantBits[square] = bishopBits;
        int bishopOccupancyCount = 1 << bishopBits;
        bishopOccupancies[square].resize(bishopOccupancyCount);
        bishopAttacks[square].resize(bishopOccupancyCount);

        for (int i = 0; i < bishopOccupancyCount; i++)
        {
            uint64_t occ = setOccupancy(i, bishopBits, bishopMasks[square]);

            bishopOccupancies[square][i] = occ;
            bishopAttacks[square][i] = generateBishopAttack(square, occ);
        }

        bishopMagics[square] = findMagicNumber(square, bishopBits, true);
    }
}

uint64_t MagicBitboards::findMagicNumber(int square,
                                         int relevantBits,
                                         bool bishop)
{
    const int occupancyCount = 1 << relevantBits;

    std::vector<uint64_t> used(occupancyCount);

    for (int attempt = 0; attempt < 100000000; attempt++)
    {
        uint64_t magic = randomMagic();

        // Skip bad candidates (optional but speeds things up)
        if (countBits((magic * (bishop ? bishopMasks[square]
                                       : rookMasks[square])) &
                      0xFF00000000000000ULL) < 6)
            continue;

        std::fill(used.begin(), used.end(), 0ULL);

        bool fail = false;

        for (int i = 0; i < occupancyCount; i++)
        {
            uint64_t occupancy =
                bishop ? bishopOccupancies[square][i]
                       : rookOccupancies[square][i];

            uint64_t attack =
                bishop ? bishopAttacks[square][i]
                       : rookAttacks[square][i];

            int index =
                (occupancy * magic) >> (64 - relevantBits);

            if (used[index] == 0ULL)
            {
                used[index] = attack;
            }
            else if (used[index] != attack)
            {
                fail = true;
                break;
            }
        }

        if (!fail)
            return magic;
    }

    return 0ULL;
}

uint64_t MagicBitboards::randomU64()
{
    static std::mt19937_64 rng(123456789); // fixed seed for reproducibility
    return rng();
}
uint64_t MagicBitboards::randomMagic()
{
    return randomU64() & randomU64() & randomU64();
}

bool MagicBitboards::verifyMagicNumbers()
{
    for (int square = 0; square < 64; square++)
    {
        // ---------- Rook ----------
        {
            int relevantBits = rookRelevantBits[square];
            int occupancyCount = 1 << relevantBits;

            std::vector<uint64_t> table(occupancyCount, UINT64_MAX);

            for (int i = 0; i < occupancyCount; i++)
            {
                uint64_t occupancy = rookOccupancies[square][i];
                uint64_t attack = rookAttacks[square][i];

                int index =
                    (occupancy * rookMagics[square]) >>
                    (64 - relevantBits);
                if (table[index] == UINT64_MAX)
                {
                    table[index] = attack;
                }
                else if (table[index] != attack)
                {
                    std::cout << "Rook magic failed on square "
                              << square << '\n';
                    return false;
                }
            }
        }

        // ---------- Bishop ----------
        {
            int relevantBits = bishopRelevantBits[square];
            int occupancyCount = 1 << relevantBits;

            std::vector<uint64_t> table(occupancyCount, UINT64_MAX);

            for (int i = 0; i < occupancyCount; i++)
            {
                uint64_t occupancy = bishopOccupancies[square][i];
                uint64_t attack = bishopAttacks[square][i];

                int index =
                    (occupancy * bishopMagics[square]) >>
                    (64 - relevantBits);

                if (table[index] == UINT64_MAX)
                {
                    table[index] = attack;
                }
                else if (table[index] != attack)
                {
                    std::cout << "Bishop magic failed on square "
                              << square << '\n';
                    return false;
                }
            }
        }
    }

    std::cout << "All magic numbers verified!\n";
    return true;
}

uint64_t MagicBitboards::getRookAttacks(int square, uint64_t occupancy)
{
    occupancy &= rookMasks[square];
    occupancy *= rookMagics[square];
    occupancy >>= (64 - rookRelevantBits[square]);

    return rookAttackTable[square][occupancy];
}

uint64_t MagicBitboards::getBishopAttacks(int square, uint64_t occupancy)
{
    occupancy &= bishopMasks[square];
    occupancy *= bishopMagics[square];
    occupancy >>= (64 - bishopRelevantBits[square]);

    return bishopAttackTable[square][occupancy];
}

uint64_t MagicBitboards::getQueenAttacks(int square, uint64_t occupancy)
{
    return getRookAttacks(square, occupancy) |
           getBishopAttacks(square, occupancy);
}

void MagicBitboards::buildMagicAttackTables()
{
    for (int square = 0; square < 64; square++)
    {
        // -------------------- Rooks --------------------
        int rookBits = rookRelevantBits[square];
        int rookOccupancyCount = 1 << rookBits;

        rookAttackTable[square].clear();
        rookAttackTable[square].resize(rookOccupancyCount);

        for (int i = 0; i < rookOccupancyCount; i++)
        {
            uint64_t occupancy = rookOccupancies[square][i];
            uint64_t attack = rookAttacks[square][i];

            uint64_t index =
                (occupancy * rookMagics[square]) >>
                (64 - rookBits);

            rookAttackTable[square][index] = attack;
        }

        // -------------------- Bishops --------------------
        int bishopBits = bishopRelevantBits[square];
        int bishopOccupancyCount = 1 << bishopBits;

        bishopAttackTable[square].clear();
        bishopAttackTable[square].resize(bishopOccupancyCount);

        for (int i = 0; i < bishopOccupancyCount; i++)
        {
            uint64_t occupancy = bishopOccupancies[square][i];
            uint64_t attack = bishopAttacks[square][i];

            uint64_t index =
                (occupancy * bishopMagics[square]) >>
                (64 - bishopBits);

            bishopAttackTable[square][index] = attack;
        }
    }
}