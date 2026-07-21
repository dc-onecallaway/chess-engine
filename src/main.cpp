#include <iostream>
#include "../include/Board.h"
#include "../include/AttackTables.h"
#include "../include/MoveGenerator.h"

int main()
{
    AttackTables::initialize();
    std::cout << "Chess Engine Started!" << std::endl;
    Board b1;
    MoveGenerator generator;
    b1.initialize();
    b1.print();
    AttackTables::print(28);
    std::vector<Move> moves;
    generator.generateKnightMoves(b1, moves);

    for (Move temp : moves)
    {
        std::cout << (int)(temp.getFrom()) << " - " << (int)(temp.getTo()) << std::endl;
    }
    return 0;
}