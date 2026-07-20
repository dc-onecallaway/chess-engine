#include <iostream>
#include "../include/Board.h"

int main()
{
    std::cout << "Chess Engine Started!" << std::endl;
    Board b1;
    b1.initialize();
    b1.print();
    return 0;
}