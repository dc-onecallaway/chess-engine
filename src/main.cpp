#include <iostream>
#include "../include/Board.h"
#include "../include/AttackTables.h"

int main()
{
    AttackTables::initialize();
    std::cout << "Chess Engine Started!" << std::endl;
    Board b1;
    b1.initialize();
    b1.print();
    AttackTables::print(28);
    return 0;
}