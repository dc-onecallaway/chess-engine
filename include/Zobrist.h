#pragma once

#include <cstdint>
#include <array>

class Zobrist
{
public:
    static std::array<std::array<uint64_t, 64>, 12> pieceKeys;

    static uint64_t sideKey;

    static std::array<uint64_t, 16> castleKeys;

    static std::array<uint64_t, 8> enPassantKeys;

    static void initialize();
};