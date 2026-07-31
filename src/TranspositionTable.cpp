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
                               int score, int ply,
                               TTFlag flag, const Move &bestMove)
{
    uint64_t index = key & mask;
    constexpr int MATE_SCORE = 100000;
    constexpr int MATE_THRESHOLD = 99000;

    // Replace only if this entry is empty
    // or the new search is deeper.
    if (table[index].depth <= depth)
    {
        table[index].key = key;
        table[index].depth = depth;
        if (score > MATE_THRESHOLD)
            score += ply;
        if (score < -MATE_THRESHOLD)
            score -= ply;
        table[index].score = score;
        table[index].flag = flag;
        table[index].bestMove = bestMove;
    }
}

bool TranspositionTable::probe(uint64_t key, int ply, TTEntry &entry)
{
    uint64_t index = key & mask;

    if (table[index].key == key)
    {
        entry = table[index];

        constexpr int MATE_THRESHOLD = 99000;

        if (entry.score > MATE_THRESHOLD)
            entry.score -= ply;
        else if (entry.score < -MATE_THRESHOLD)
            entry.score += ply;

        return true;
    }

    return false;
}