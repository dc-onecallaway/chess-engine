#pragma once

#include <cstdint>
#include <vector>
#include <Move.h>

enum class TTFlag
{
    Exact,
    LowerBound,
    UpperBound
};

struct TTEntry
{
    uint64_t key = 0;
    int depth = -1;
    int score = 0;
    TTFlag flag = TTFlag::Exact;
    Move bestMove;
};

class TranspositionTable
{
public:
    static void initialize(size_t mb = 64);

    static void store(uint64_t key,
                      int depth,
                      int score, int ply,
                      TTFlag flag, const Move &bestMove);

    static bool probe(uint64_t key, int ply,
                      TTEntry &entry);
    static void clear();

private:
    static std::vector<TTEntry> table;
    static uint64_t mask;
};