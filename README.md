# ♟️ OneCallEngine

A chess engine written in **modern C++** from scratch using **bitboards**, **magic bitboards**, **alpha-beta pruning**, and **iterative deepening search**.

The goal of this project is to understand how modern chess engines work by implementing every major component from scratch instead of relying on existing chess libraries.

---

## Features

### Board Representation

- Bitboard-based board representation
- Efficient piece lookup using 64-bit integers
- Complete legal move generation
- Make / Undo move implementation

### Move Generation

- Magic Bitboards for sliding pieces
- Precomputed attack tables for:
  - Pawns
  - Knights
  - Kings
- Legal move filtering
- Castling
- En Passant
- Pawn Promotions

### Search

- Minimax Search
- Alpha-Beta Pruning
- Quiescence Search
- Iterative Deepening
- Transposition Table
- Hash Move Ordering

### Position Hashing

- Zobrist Hashing
- Incremental hash updates
- Transposition Table lookups

### Engine Interface

- UCI (Universal Chess Interface) compatible
- Compatible with GUI applications such as:
  - Banksia GUI
  - Arena
  - Cute Chess

---

## Project Structure

```
ChessEngine/
│
├── include/
│   ├── Board.h
│   ├── Move.h
│   ├── Search.h
│   ├── Evaluation.h
│   ├── MoveGenerator.h
│   ├── TranspositionTable.h
│   ├── MagicBitboards.h
│   ├── Zobrist.h
│   ├── UCI.h
│   └── ...
│
├── src/
│   ├── Board.cpp
│   ├── Search.cpp
│   ├── MoveGenerator.cpp
│   ├── Evaluation.cpp
│   ├── MagicBitboards.cpp
│   ├── Zobrist.cpp
│   ├── UCI.cpp
│   └── ...
│
└── CMakeLists.txt
```

---

## Algorithms Used

- Bitboards
- Magic Bitboards
- Alpha-Beta Pruning
- Quiescence Search
- Iterative Deepening
- Zobrist Hashing
- Transposition Tables
- Hash Move Ordering

---

## Build

Clone the repository

```bash
git clone https://github.com/<your-username>/OneCallEngine.git
cd OneCallEngine
```

Build

```bash
mkdir build
cd build
cmake ..
make
```

Run

```bash
./ChessEngine
```

---

## Current Strength

The engine currently includes:

- Legal move generation
- Alpha-beta search
- Quiescence search
- Iterative deepening
- Transposition table
- Hash move ordering

Development is ongoing, with stronger search heuristics and evaluation improvements planned.

---

## Planned Improvements

- [ ] Mate score normalization
- [ ] Killer Move Heuristic
- [ ] History Heuristic
- [ ] Principal Variation Search (PVS)
- [ ] Aspiration Windows
- [ ] Null Move Pruning
- [ ] Late Move Reductions (LMR)
- [ ] Static Exchange Evaluation (SEE)
- [ ] Improved Evaluation Function
- [ ] NNUE Evaluation

---

## Learning Objectives

This project was built to gain hands-on experience with:

- Chess engine architecture
- Game tree search
- Move generation
- Search optimization
- Low-level bit manipulation
- Performance-oriented C++

---

## Technologies

- C++17
- STL
- CMake

---

## References

- Chess Programming Wiki
- Stockfish (for studying concepts)
- Magic Bitboard literature
- UCI Protocol Specification

---

## Author

**Deepak Chauhan**

B.Tech Mathematics & Computing  
Delhi Technological University (DTU)

GitHub: https://github.com/dc-onecallaway

---
