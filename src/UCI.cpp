#include "../include/UCI.h"
#include "../include/UCIUtils.h"
#include <iostream>
#include <sstream>
#include "../include/Board.h"
#include "../include/Search.h"

void UCI::loop()
{

    Board board;
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (line == "uci")
        {
            std::cout << "id name OneCallEngine" << std::endl;
            std::cout << "id author Deepak Chauhan" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (line == "isready")
        {
            std::cout << "readyok" << std::endl;
        }
        else if (line == "quit")
        {
            break;
        }
        else if (line.rfind("position", 0) == 0)
        {
            std::stringstream ss(line);
            std::string token;
            ss >> token;
            ss >> token;
            if (token == "startpos")
            {
                board.initialize();
            }
            while (ss >> token)
            {
                if (token == "moves")
                    continue;
                Move move = UCIUtils::parseMove(board, token);
                board.makeMove(move);
            }
        }
        else if (line.rfind("go", 0) == 0)
        {
            std::stringstream ss(line);

            std::string token;
            int depth = 4; // default

            ss >> token; // go
            ss >> token; // depth

            if (token == "depth")
            {
                ss >> depth;
            }

            Search search;

            Move best = search.findBestMove(board, depth);

            std::cout << "bestmove "
                      << UCIUtils::moveToString(best)
                      << std::endl;
        }
    }
}