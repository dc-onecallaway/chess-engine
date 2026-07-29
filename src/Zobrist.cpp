#include "../include/Zobrist.h"

#include <random>

std::array<std::array<uint64_t, 64>, 12> Zobrist::pieceKeys;

uint64_t Zobrist::sideKey;

std::array<uint64_t, 16> Zobrist::castleKeys;

std::array<uint64_t, 8> Zobrist::enPassantKeys;

void Zobrist::initialize()
{
    std::mt19937_64 rng(20250727);

    auto rand64 = [&]()
    {
        return rng();
    };

    for (int piece = 0; piece < 12; piece++)
        for (int sq = 0; sq < 64; sq++)
            pieceKeys[piece][sq] = rand64();

    sideKey = rand64();

    for (int i = 0; i < 16; i++)
        castleKeys[i] = rand64();

    for (int i = 0; i < 8; i++)
        enPassantKeys[i] = rand64();
}