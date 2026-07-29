#include "../include/TranspositionTable.h"

std::vector<TTEntry> TranspositionTable::table;
uint64_t TranspositionTable::mask = 0;

void TranspositionTable::initialize(size_t mb)
{
    size_t bytes = mb * 1024ULL * 1024ULL;
    size_t entries = bytes / sizeof(TTEntry);

    // Make table size a power of two
    size_t power = 1;
    while (power < entries)
        power <<= 1;
    power >>= 1;

    table.clear();
    table.resize(power);

    mask = power - 1;
}

void TranspositionTable::store(uint64_t key,
                               int depth,
                               int score,
                               TTFlag flag)
{
    uint64_t index = key & mask;

    // Replace only if this entry is empty
    // or the new search is deeper.
    if (table[index].depth <= depth)
    {
        table[index].key = key;
        table[index].depth = depth;
        table[index].score = score;
        table[index].flag = flag;
    }
}

bool TranspositionTable::probe(uint64_t key, TTEntry &entry)
{
    uint64_t index = key & mask;

    if (table[index].key == key)
    {
        entry = table[index];
        return true;
    }

    return false;
}