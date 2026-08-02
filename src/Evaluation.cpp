#include "../include/Evaluation.h"
#include "../include/PieceSquareTables.h"
#include "../include/AttackTables.h"
#include "../include/MagicBitboards.h"

static constexpr int PassedBonus[8] =
    {
        0,
        10,
        20,
        35,
        60,
        100,
        180,
        0};

static constexpr int DoubledPawnPenalty = 15;
static constexpr int OpenFileBonus = 20;
static constexpr int SemiOpenBonus = 10;
static constexpr int IsolatedPawnPenalty = 12;
static constexpr int PawnShieldBonus = 12;
static constexpr int MissingPawnPenalty = 8;
static constexpr int RookSeventhRankBonus = 25;

int Evaluation::evaluate(const Board &board)
{
    // TODO
    int score = 0;

    int whitePawnFileCount[8] = {0};
    int blackPawnFileCount[8] = {0};

    uint64_t whitePawn = board.getPieceBitboard(Piece::WhitePawn);
    while (whitePawn)
    {
        int square = __builtin_ctzll(whitePawn);
        score += 100;
        score += PieceSquareTables::PawnTable[PieceSquareTables::mirrorSquare(square)];

        // passed pawn evaluation
        bool passed = true;
        const int file = square % 8;
        const int rank = square / 8;

        whitePawnFileCount[file]++;

        for (int r = rank + 1; r < 8 && passed; r++)
        {
            for (int f = std::max(0, file - 1); f <= std::min(7, file + 1); f++)
            {
                int sq = r * 8 + f;
                if (board.getPieceOnSquare(sq) == Piece::BlackPawn)
                {
                    passed = false;
                    break;
                }
            }
        }
        if (passed)
        {
            score += PassedBonus[rank];
        }
        whitePawn &= (whitePawn - 1);
    }

    uint64_t blackPawn = board.getPieceBitboard(Piece::BlackPawn);
    while (blackPawn)
    {
        int square = __builtin_ctzll(blackPawn);
        score -= 100;
        score -= PieceSquareTables::PawnTable[square];

        // passed pawn evaluation
        bool passed = true;
        const int file = square % 8;
        const int rank = square / 8;

        blackPawnFileCount[file]++;

        for (int r = rank - 1; r >= 0 && passed; r--)
        {
            for (int f = std::max(0, file - 1); f <= std::min(7, file + 1); f++)
            {
                int sq = r * 8 + f;
                if (board.getPieceOnSquare(sq) == Piece::WhitePawn)
                {
                    passed = false;
                    break;
                }
            }
        }
        if (passed)
        {
            score -= PassedBonus[7 - rank];
        }

        blackPawn &= (blackPawn - 1);
    }

    for (int file = 0; file < 8; file++)
    {
        // Doubled pawns
        if (whitePawnFileCount[file] > 1)
            score -= DoubledPawnPenalty * (whitePawnFileCount[file] - 1);

        if (blackPawnFileCount[file] > 1)
            score += DoubledPawnPenalty * (blackPawnFileCount[file] - 1);

        // Isolated white pawns
        if (whitePawnFileCount[file] > 0)
        {
            bool left = (file > 0 && whitePawnFileCount[file - 1] > 0);
            bool right = (file < 7 && whitePawnFileCount[file + 1] > 0);

            if (!left && !right)
                score -= IsolatedPawnPenalty * whitePawnFileCount[file];
        }

        // Isolated black pawns
        if (blackPawnFileCount[file] > 0)
        {
            bool left = (file > 0 && blackPawnFileCount[file - 1] > 0);
            bool right = (file < 7 && blackPawnFileCount[file + 1] > 0);

            if (!left && !right)
                score += IsolatedPawnPenalty * blackPawnFileCount[file];
        }
    }

    uint64_t whiteKnight = board.getPieceBitboard(Piece::WhiteKnight);
    while (whiteKnight)
    {
        int square = __builtin_ctzll(whiteKnight);
        score += 320;
        score += PieceSquareTables::KnightTable[PieceSquareTables::mirrorSquare(square)];

        // Mobility
        uint64_t attacks = AttackTables::knightAttacks[square];
        attacks &= ~board.getWhiteOccupancy(); // can't move onto own pieces
        score += 4 * __builtin_popcountll(attacks);

        whiteKnight &= (whiteKnight - 1);
    }

    uint64_t blackKnight = board.getPieceBitboard(Piece::BlackKnight);
    while (blackKnight)
    {
        int square = __builtin_ctzll(blackKnight);
        score -= 320;
        score -= PieceSquareTables::KnightTable[square];

        // Mobility
        uint64_t attacks = AttackTables::knightAttacks[square];
        attacks &= ~board.getBlackOccupancy(); // can't move onto own pieces
        score -= 4 * __builtin_popcountll(attacks);

        blackKnight &= (blackKnight - 1);
    }

    uint64_t whiteBishop = board.getPieceBitboard(Piece::WhiteBishop);
    while (whiteBishop)
    {
        int square = __builtin_ctzll(whiteBishop);
        score += 330;
        score += PieceSquareTables::BishopTable[PieceSquareTables::mirrorSquare(square)];

        // Mobility
        uint64_t attacks = MagicBitboards::getBishopAttacks(square, board.getOccupied());
        attacks &= ~board.getWhiteOccupancy();
        score += 5 * __builtin_popcountll(attacks);

        whiteBishop &= (whiteBishop - 1);
    }

    uint64_t blackBishop = board.getPieceBitboard(Piece::BlackBishop);
    while (blackBishop)
    {
        int square = __builtin_ctzll(blackBishop);
        score -= 330;
        score -= PieceSquareTables::BishopTable[square];

        // Mobility
        uint64_t attacks = MagicBitboards::getBishopAttacks(square, board.getOccupied());
        attacks &= ~board.getBlackOccupancy();
        score -= 5 * __builtin_popcountll(attacks);

        blackBishop &= (blackBishop - 1);
    }

    // Bishop Pair

    int whiteBishops = __builtin_popcountll(board.getPieceBitboard(Piece::WhiteBishop));
    int blackBishops = __builtin_popcountll(board.getPieceBitboard(Piece::BlackBishop));

    if (whiteBishops >= 2)
        score += 30;

    if (blackBishops >= 2)
        score -= 30;

    uint64_t whiteRook = board.getPieceBitboard(Piece::WhiteRook);
    while (whiteRook)
    {
        int square = __builtin_ctzll(whiteRook);
        score += 500;
        score += PieceSquareTables::RookTable[PieceSquareTables::mirrorSquare(square)];

        // Mobility
        uint64_t attacks = MagicBitboards::getRookAttacks(square, board.getOccupied());
        attacks &= ~board.getWhiteOccupancy();
        score += 2 * __builtin_popcountll(attacks);

        // Open File and Semi Open File
        int file = square % 8;

        // Rook on 7th Rank
        int rank = square / 8;
        if (rank == 6)
        {
            uint64_t pawns = board.getPieceBitboard(Piece::BlackPawn);

            if (pawns & 0x00FF000000000000ULL)
                score += RookSeventhRankBonus;
        }

        if (whitePawnFileCount[file] == 0)
        {
            if (blackPawnFileCount[file] == 0)
                score += OpenFileBonus;
            else
                score += SemiOpenBonus;
        }

        whiteRook &= (whiteRook - 1);
    }

    uint64_t blackRook = board.getPieceBitboard(Piece::BlackRook);
    while (blackRook)
    {
        int square = __builtin_ctzll(blackRook);
        score -= 500;
        score -= PieceSquareTables::RookTable[square];

        // Mobility
        uint64_t attacks = MagicBitboards::getRookAttacks(square, board.getOccupied());
        attacks &= ~board.getBlackOccupancy();
        score -= 2 * __builtin_popcountll(attacks);

        // Open File and Semi Open File
        int file = square % 8;

        // Rook on 2th Rank
        int rank = square / 8;
        if (rank == 1)
        {
            uint64_t pawns = board.getPieceBitboard(Piece::WhitePawn);

            if (pawns & 0x000000000000FF00ULL)
                score -= RookSeventhRankBonus;
        }

        if (whitePawnFileCount[file] == 0)
        {
            if (blackPawnFileCount[file] == 0)
                score -= OpenFileBonus;
            else
                score -= SemiOpenBonus;
        }

        blackRook &= (blackRook - 1);
    }
    uint64_t whiteQueen = board.getPieceBitboard(Piece::WhiteQueen);
    while (whiteQueen)
    {
        int square = __builtin_ctzll(whiteQueen);
        score += 900;
        score += PieceSquareTables::QueenTable[PieceSquareTables::mirrorSquare(square)];

        // Mobility
        uint64_t attacks = MagicBitboards::getQueenAttacks(square, board.getOccupied());
        attacks &= ~board.getWhiteOccupancy();
        score += __builtin_popcountll(attacks);

        whiteQueen &= (whiteQueen - 1);
    }

    uint64_t blackQueen = board.getPieceBitboard(Piece::BlackQueen);
    while (blackQueen)
    {
        int square = __builtin_ctzll(blackQueen);
        score -= 900;
        score -= PieceSquareTables::QueenTable[square];

        // Mobility
        uint64_t attacks = MagicBitboards::getQueenAttacks(square, board.getOccupied());
        attacks &= ~board.getBlackOccupancy();
        score -= __builtin_popcountll(attacks);

        blackQueen &= (blackQueen - 1);
    }
    int endgameMaterial = 0;

    endgameMaterial +=
        320 * __builtin_popcountll(board.getPieceBitboard(Piece::WhiteKnight)) +
        330 * __builtin_popcountll(board.getPieceBitboard(Piece::WhiteBishop)) +
        500 * __builtin_popcountll(board.getPieceBitboard(Piece::WhiteRook)) +
        900 * __builtin_popcountll(board.getPieceBitboard(Piece::WhiteQueen));

    endgameMaterial +=
        320 * __builtin_popcountll(board.getPieceBitboard(Piece::BlackKnight)) +
        330 * __builtin_popcountll(board.getPieceBitboard(Piece::BlackBishop)) +
        500 * __builtin_popcountll(board.getPieceBitboard(Piece::BlackRook)) +
        900 * __builtin_popcountll(board.getPieceBitboard(Piece::BlackQueen));

    bool endgame = endgameMaterial <= 2600;
    int whiteKingSquare = board.getKingSquare(true);
    int blackKingSquare = board.getKingSquare(false);

    if (endgame)
    {
        score += PieceSquareTables::KingEndgameTable[PieceSquareTables::mirrorSquare(whiteKingSquare)];
        score -= PieceSquareTables::KingEndgameTable[blackKingSquare];
    }
    else
    {
        int file = whiteKingSquare % 8;
        int rank = whiteKingSquare / 8;

        if (rank <= 6) // there is a rank in front
        {
            for (int df = -1; df <= 1; df++)
            {
                int nf = file + df;
                if (nf < 0 || nf > 7)
                    continue;

                int sq = (rank + 1) * 8 + nf;

                if (board.getPieceOnSquare(sq) == Piece::WhitePawn)
                    score += PawnShieldBonus;
                else
                    score -= MissingPawnPenalty;
            }
        }

        file = blackKingSquare % 8;
        rank = blackKingSquare / 8;

        if (rank >= 1)
        {
            for (int df = -1; df <= 1; df++)
            {
                int nf = file + df;
                if (nf < 0 || nf > 7)
                    continue;

                int sq = (rank - 1) * 8 + nf;

                if (board.getPieceOnSquare(sq) == Piece::BlackPawn)
                    score -= PawnShieldBonus;
                else
                    score += MissingPawnPenalty;
            }
        }
        score += PieceSquareTables::KingTable[PieceSquareTables::mirrorSquare(whiteKingSquare)];
        score -= PieceSquareTables::KingTable[blackKingSquare];
    }
    return score;
}