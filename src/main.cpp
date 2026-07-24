#include <iostream>
#include "../include/Board.h"
#include "../include/AttackTables.h"
#include "../include/MoveGenerator.h"
#include "../include/Perft.h"
#include "../include/Evaluation.h"
#include "../include/Search.h"
#include "../include/UCI.h"
#include "../include/UCIUtils.h"
#include <chrono>

int main()
{
    AttackTables::initialize();
    std::cout << "Chess Engine Started!" << std::endl;
    Board board;
    MoveGenerator generator;
    board.initialize();
    board.print();
    int turns = 20;
    Search search;

    while (turns--)
    {
        // std::string s;
        // std::getline(std::cin, s);
        // Move move = UCIUtils::parseMove(board, s);
        // board.makeMove(move);
        // board.print(); // show your move
        std::cout << "Searching...\n";
        auto start = std::chrono::steady_clock::now();
        Move best = search.findBestMove(board, 5);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Search took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
        std::cout << "Search finished\n";
        std::cout << "Engine plays: " << UCIUtils::moveToString(best) << "\n";
        board.makeMove(best);
        board.print();
    }

    return 0;
}

// int main()
// {
//     AttackTables::initialize();

//     UCI uci;
//     uci.loop();
// }