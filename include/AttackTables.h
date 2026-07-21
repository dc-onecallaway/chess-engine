#ifndef ATTACKTABLES_H
#define ATTACKTABLES_H

#include <cstdint>

namespace AttackTables
{
    extern uint64_t knightAttacks[64];
    extern uint64_t kingAttacks[64];
    extern uint64_t whitePawnAttacks[64];
    extern uint64_t blackPawnAttacks[64];

    void initialize();
    void print(uint64_t bitboard);
    uint64_t getBishopAttacks(int square, uint64_t occupied);
    uint64_t getRookAttacks(int square, uint64_t occupied);
    uint64_t getQueenAttacks(int square, uint64_t occupied);
}
#endif