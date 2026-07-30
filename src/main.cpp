#include <iostream>
#include "../include/Board.h"
#include "../include/AttackTables.h"
#include "../include/MoveGenerator.h"
#include "../include/Perft.h"
#include "../include/Evaluation.h"
#include "../include/Search.h"
#include "../include/UCI.h"
#include "../include/UCIUtils.h"
#include "../include/MagicBitboards.h"
#include "../include/Zobrist.h"
#include "../include/TranspositionTable.h"
#include <chrono>

// int main()
// {
//     AttackTables::initialize();
//     Zobrist::initialize();
//     std::cout << "Chess Engine Started!" << std::endl;
//     Board board;
//     MoveGenerator generator;
//     board.initialize();
//     MagicBitboards::initialize();
//     // board.print();
//     TranspositionTable::initialize();

//     // TTEntry entry;

//     // TranspositionTable::store(
//     //     123456789ULL,
//     //     5,
//     //     100,
//     //     TTFlag::Exact);

//     // if (TranspositionTable::probe(123456789ULL, entry))
//     // {
//     //     std::cout << "Found!\n";
//     //     std::cout << entry.depth << "\n";
//     //     std::cout << entry.score << "\n";
//     // }
//     // else
//     // {
//     //     std::cout << "Not Found!\n";
//     // }
//     int turns = 20;
//     Search search;

//     while (turns--)
//     {
//         // std::string s;
//         // std::getline(std::cin, s);
//         // Move move = UCIUtils::parseMove(board, s);
//         // board.makeMove(move);
//         // board.print(); // show your move
//         std::cout << "Searching...\n";
//         auto start = std::chrono::steady_clock::now();
//         Move best = search.findBestMove(board, 6);
//         auto end = std::chrono::steady_clock::now();
//         std::cout << "Search took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
//         std::cout << "Search finished\n";
//         std::cout << "Engine plays: " << UCIUtils::moveToString(best) << "\n";
//         board.makeMove(best);
//         board.print();
//     }

//     // if (!MagicBitboards::verifyMagicNumbers())
//     // {
//     //     std::cout << "Verification failed!\n";
//     // }
//     return 0;
// }

int main()
{
    AttackTables::initialize();
    Zobrist::initialize();
    MagicBitboards::initialize();
    TranspositionTable::initialize();

    UCI uci;
    uci.loop();
}