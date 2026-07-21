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
    std::vector<Move> moves;
    // AttackTables::print(AttackTables::kingAttacks[0]);
    generator.generateKingMoves(b1, moves);
    std::cout << moves.size() << '\n';
    // generator.generateKnightMoves(b1, moves);

    for (Move temp : moves)
    {
        std::cout << (int)(temp.getFrom()) << " - " << (int)(temp.getTo()) << std::endl;
    }

    AttackTables::print(AttackTables::whitePawnAttacks[28]);
    std::cout << '\n';
    AttackTables::print(AttackTables::blackPawnAttacks[28]);

    return 0;
}