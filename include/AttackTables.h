#ifndef ATTACKTABLES_H
#define ATTACKTABLES_H

#include <cstdint>

namespace AttackTables
{
    extern uint64_t knightAttacks[64];
    extern uint64_t kingAttacks[64];

    void initialize();
    void print(uint64_t bitboard);
}

#endif