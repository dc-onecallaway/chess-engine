#include <array>
#include <vector>
#include <cstdint>

class MagicBitboards
{
public:
    static std::array<uint64_t, 64> rookMasks;
    static std::array<uint64_t, 64> bishopMasks;

    // Relevant occupancy bit counts
    static std::array<int, 64> rookRelevantBits;
    static std::array<int, 64> bishopRelevantBits;

    // Occupancy -> Attack tables (temporary, used while building magic tables)
    static std::array<std::vector<uint64_t>, 64> rookOccupancies;
    static std::array<std::vector<uint64_t>, 64> bishopOccupancies;

    static std::array<std::vector<uint64_t>, 64> rookAttacks;
    static std::array<std::vector<uint64_t>, 64> bishopAttacks;

    static std::array<std::vector<uint64_t>, 64> rookAttackTable;
    static std::array<std::vector<uint64_t>, 64> bishopAttackTable;

    static std::array<uint64_t, 64> rookMagics;
    static std::array<uint64_t, 64> bishopMagics;
    static bool verifyMagicNumbers();
    static uint64_t getRookAttacks(int square, uint64_t occupancy);
    static uint64_t getBishopAttacks(int square, uint64_t occupancy);
    static uint64_t getQueenAttacks(int square, uint64_t occupancy);

    static void initialize();

private:
    static void initializeMasks();
    static void initializeAttackTables();
    static void buildMagicAttackTables();

    static uint64_t setOccupancy(int index, int bitsInMask, uint64_t mask);

    static int countBits(uint64_t bb);
    static int getLSBIndex(uint64_t bb);

    static uint64_t generateRookAttack(int square, uint64_t blockers);
    static uint64_t generateBishopAttack(int square, uint64_t blockers);
    static uint64_t findMagicNumber(int square, int relevantBits, bool bishop);

    static uint64_t randomU64();
    static uint64_t randomMagic();
};