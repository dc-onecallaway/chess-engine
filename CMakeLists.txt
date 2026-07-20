cmake_minimum_required(VERSION 3.20)

project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(
    ChessEngine
    src/main.cpp
    src/Board.cpp
    src/Move.cpp
    src/Piece.cpp
    src/Engine.cpp
)