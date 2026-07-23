#include <iostream>
#include "../include/Board.h"
#include "../include/AttackTables.h"
#include "../include/MoveGenerator.h"
#include "../include/Perft.h"
#include "../include/Evaluation.h"
#include "../include/Search.h"

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
        std::cout << "Searching...\n";
        Move best = search.findBestMove(board, 4);
        std::cout << "Search finished\n";
        board.makeMove(best);
        board.print();
    }

    return 0;
}