#include "../include/Board.h"
#include "../include/AttackTables.h"
#include <iostream>
#include <map>

void Board::initialize()
{
    pieces[static_cast<int>(Piece::WhitePawn)] = ((1ULL << 8) - 1ULL) << 8;
    pieces[static_cast<int>(Piece::WhiteKnight)] = (1ULL << 6) | (1ULL << 1);
    pieces[static_cast<int>(Piece::WhiteBishop)] = (1ULL << 5) | (1ULL << 2);
    pieces[static_cast<int>(Piece ::WhiteRook)] = (1ULL << 7) | (1ULL << 0);
    pieces[static_cast<int>(Piece::WhiteQueen)] = (1ULL << 3);
    pieces[static_cast<int>(Piece::WhiteKing)] = (1ULL << 4);

    pieces[static_cast<int>(Piece::BlackPawn)] = ((1ULL << 8) - 1ULL) << 48;
    pieces[static_cast<int>(Piece::BlackKnight)] = (1ULL << 57) | (1ULL << 62);
    pieces[static_cast<int>(Piece::BlackBishop)] = (1ULL << 58) | (1ULL << 61);
    pieces[static_cast<int>(Piece::BlackRook)] = (1ULL << 56) | (1ULL << 63);
    pieces[static_cast<int>(Piece::BlackQueen)] = (1ULL << 59);
    pieces[static_cast<int>(Piece::BlackKing)] = (1ULL << 60);

    whiteToMove = true;

    whiteCastleKingSide = true;
    whiteCastleQueenSide = true;
    blackCastleKingSide = true;
    blackCastleQueenSide = true;

    enPassantSquare = -1;

    halfMoveClock = 0;

    fullMoveNumber = 1;
}

void Board::print()
{

    std::map<int, char> mp;

    mp[0] = 'P';
    mp[1] = 'N';
    mp[2] = 'B';
    mp[3] = 'R';
    mp[4] = 'Q';
    mp[5] = 'K';
    mp[6] = 'p';
    mp[7] = 'n';
    mp[8] = 'b';
    mp[9] = 'r';
    mp[10] = 'q';
    mp[11] = 'k';

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int value = 8 * (7 - i) + j;
            bool printed = false;
            for (int k = 0; k < 12; k++)
            {
                if (pieces[k] & (1ULL << value))
                {
                    std::cout << mp[k] << " ";
                    printed = true;
                    break;
                }
            }
            if (printed == false)
            {
                std::cout << "." << " ";
            }
        }
        std::cout << std::endl;
    }
}

uint64_t Board::getWhiteOccupancy() const
{
    uint64_t board = 0ULL;
    for (int i = 0; i < 6; i++)
    {
        board |= pieces[i];
    }
    return board;
}
uint64_t Board::getBlackOccupancy() const
{
    uint64_t board = 0ULL;
    for (int i = 6; i < 12; i++)
    {
        board |= pieces[i];
    }
    return board;
}
uint64_t Board::getOccupied() const
{
    return (getBlackOccupancy() | getWhiteOccupancy());
}

bool Board::isWhiteToMove() const
{
    return whiteToMove;
}

uint64_t Board::getPieceBitboard(Piece piece) const
{
    return pieces[static_cast<int>(piece)];
}

int Board::getEnPassantSquare() const
{
    return enPassantSquare;
}

bool Board::isSquareAttacked(int square, bool byWhite) const
{
    uint64_t occupied = getOccupied();
    uint64_t sq = 1ULL << square;

    int base = byWhite ? 0 : 6; // your pieces[] layout: 0-5 white, 6-11 black

    // Pawns: use the OPPOSITE color's pawn-attack table from `square`,
    // since "does a white pawn attack square X" is the mirror of
    // "does square X attack-pattern (as if black) reach a white pawn"
    uint64_t pawnAttackers = byWhite
                                 ? AttackTables::blackPawnAttacks[square] & pieces[base + 0]  // WhitePawn
                                 : AttackTables::whitePawnAttacks[square] & pieces[base + 0]; // BlackPawn
    if (pawnAttackers)
        return true;

    if (AttackTables::knightAttacks[square] & pieces[base + 1])
        return true; // Knight

    uint64_t bishopQueenAttacks = AttackTables::getBishopAttacks(square, occupied);
    if (bishopQueenAttacks & (pieces[base + 2] | pieces[base + 4]))
        return true; // Bishop | Queen

    uint64_t rookQueenAttacks = AttackTables::getRookAttacks(square, occupied);
    if (rookQueenAttacks & (pieces[base + 3] | pieces[base + 4]))
        return true; // Rook | Queen

    if (AttackTables::kingAttacks[square] & pieces[base + 5])
        return true; // King

    return false;
}

bool Board::isWhiteCastleKingSide() const
{
    return whiteCastleKingSide;
}
bool Board::isWhiteCastleQueenSide() const
{
    return whiteCastleQueenSide;
}
bool Board::isBlackCastleKingSide() const
{
    return blackCastleKingSide;
}
bool Board::isBlackCastleQueenSide() const
{
    return blackCastleQueenSide;
}

void Board::removePiece(Piece piece, int square)
{
    pieces[static_cast<int>(piece)] &= ~(1ULL << square);
}

void Board::addPiece(Piece piece, int square)
{
    pieces[static_cast<int>(piece)] |= (1ULL << square);
}

Piece Board::getPieceOnSquare(int square) const
{
    uint64_t board = getOccupied();
    uint64_t mask = (1ULL << square);
    if (!(board & mask))
    {
        return Piece::None;
    }
    if (getWhiteOccupancy() & mask)
    {
        if (pieces[static_cast<int>(Piece::WhitePawn)] & mask)
        {
            return Piece::WhitePawn;
        }
        else if (pieces[static_cast<int>(Piece::WhiteKnight)] & mask)
        {
            return Piece::WhiteKnight;
        }
        else if (pieces[static_cast<int>(Piece::WhiteBishop)] & mask)
        {
            return Piece::WhiteBishop;
        }
        else if (pieces[static_cast<int>(Piece::WhiteQueen)] & mask)
        {
            return Piece::WhiteQueen;
        }
        else if (pieces[static_cast<int>(Piece::WhiteRook)] & mask)
        {
            return Piece::WhiteRook;
        }
        else if (pieces[static_cast<int>(Piece::WhiteKing)] & mask)
        {
            return Piece::WhiteKing;
        }

        return Piece::None;
    }
    else
    {
        if (pieces[static_cast<int>(Piece::BlackPawn)] & mask)
        {
            return Piece::BlackPawn;
        }
        else if (pieces[static_cast<int>(Piece::BlackKnight)] & mask)
        {
            return Piece::BlackKnight;
        }
        else if (pieces[static_cast<int>(Piece::BlackBishop)] & mask)
        {
            return Piece::BlackBishop;
        }
        else if (pieces[static_cast<int>(Piece::BlackQueen)] & mask)
        {
            return Piece::BlackQueen;
        }
        else if (pieces[static_cast<int>(Piece::BlackRook)] & mask)
        {
            return Piece::BlackRook;
        }
        else if (pieces[static_cast<int>(Piece::BlackKing)] & mask)
        {
            return Piece::BlackKing;
        }

        return Piece::None;
    }
}

void Board::makeMove(const Move &move)
{
    // 0. Undo Information
    UndoInfo undo;

    undo.move = move;
    undo.movedPiece = getPieceOnSquare(move.getFrom());
    if (move.getMoveType() == MoveType::EnPassant)
    {
        if (whiteToMove)
        {
            undo.capturedPiece = getPieceOnSquare(move.getTo() - 8);
        }
        else
        {
            undo.capturedPiece = getPieceOnSquare(move.getTo() + 8);
        }
    }
    else
    {
        undo.capturedPiece = getPieceOnSquare(move.getTo());
    }

    undo.whiteCastleKingSide = whiteCastleKingSide;
    undo.whiteCastleQueenSide = whiteCastleQueenSide;
    undo.blackCastleKingSide = blackCastleKingSide;
    undo.blackCastleQueenSide = blackCastleQueenSide;

    undo.enPassantSquare = enPassantSquare;
    undo.halfMoveClock = halfMoveClock;
    undo.fullMoveNumber = fullMoveNumber;

    history.push_back(undo);

    // 1. Find moving piece
    Piece movingPiece = getPieceOnSquare(move.getFrom());

    // 2. Remove moving piece
    removePiece(movingPiece, move.getFrom());

    Piece capturedPiece = getPieceOnSquare(move.getTo());
    MoveType mType = move.getMoveType();

    if (mType == MoveType::Quiet)
    {

        addPiece(movingPiece, move.getTo());
    }
    else if (mType == MoveType::Capture)
    {
        removePiece(capturedPiece, move.getTo());
        addPiece(movingPiece, move.getTo());
    }
    else if (mType == MoveType::Promotion)
    {
        addPiece(move.getPromotionPiece(), move.getTo());
    }
    else if (mType == MoveType::PromotionCapture)
    {
        removePiece(capturedPiece, move.getTo());
        addPiece(move.getPromotionPiece(), move.getTo());
    }
    else if (mType == MoveType::EnPassant)
    {
        int removingSquare = whiteToMove ? move.getTo() - 8 : move.getTo() + 8;
        capturedPiece = getPieceOnSquare(removingSquare);
        removePiece(capturedPiece, removingSquare);
        addPiece(movingPiece, move.getTo());
    }
    else if (mType == MoveType::KingCastle)
    {
        addPiece(movingPiece, move.getTo());
        Piece rook = getPieceOnSquare(move.getFrom() + 3);
        removePiece(rook, move.getFrom() + 3);
        addPiece(rook, move.getTo() - 1);
    }
    else if (mType == MoveType::QueenCastle)
    {
        addPiece(movingPiece, move.getTo());
        Piece rook = getPieceOnSquare(move.getFrom() - 4);
        removePiece(rook, move.getFrom() - 4);
        addPiece(rook, move.getTo() + 1);
    }

    // 6. Update castling rights
    if (whiteCastleKingSide)
    {
        if (movingPiece == Piece::WhiteKing || ((movingPiece == Piece::WhiteRook) && move.getFrom() == 7) || ((capturedPiece == Piece::WhiteRook) && move.getTo() == 7))
        {
            whiteCastleKingSide = false;
        }
    }
    if (whiteCastleQueenSide)
    {
        if (movingPiece == Piece::WhiteKing || ((movingPiece == Piece::WhiteRook) && move.getFrom() == 0) || ((capturedPiece == Piece::WhiteRook) && move.getTo() == 0))
        {
            whiteCastleQueenSide = false;
        }
    }
    if (blackCastleKingSide)
    {
        if (movingPiece == Piece::BlackKing || ((movingPiece == Piece::BlackRook) && move.getFrom() == 63) || ((capturedPiece == Piece::BlackRook) && move.getTo() == 63))
        {
            blackCastleKingSide = false;
        }
    }
    if (blackCastleQueenSide)
    {
        if (movingPiece == Piece::BlackKing || ((movingPiece == Piece::BlackRook) && move.getFrom() == 56) || ((capturedPiece == Piece::BlackRook) && move.getTo() == 56))
        {
            blackCastleQueenSide = false;
        }
    }

    // 7. Update en passant square
    enPassantSquare = -1;
    if (movingPiece == Piece::WhitePawn)
    {
        if (move.getFrom() + 16 == move.getTo())
        {
            enPassantSquare = move.getFrom() + 8;
        }
    }
    else if (movingPiece == Piece::BlackPawn)
    {
        if (move.getFrom() - 16 == move.getTo())
        {
            enPassantSquare = move.getFrom() - 8;
        }
    }

    // 8. Update halfmove clock
    bool pawnMove = movingPiece == Piece::WhitePawn ||
                    movingPiece == Piece::BlackPawn;

    bool capture = mType == MoveType::Capture ||
                   mType == MoveType::EnPassant ||
                   mType == MoveType::PromotionCapture;

    halfMoveClock = (pawnMove || capture) ? 0 : halfMoveClock + 1;

    // 9. Update fullmove number
    if (!whiteToMove)
    {
        fullMoveNumber++;
    }

    // 10. Switch side
    whiteToMove = !whiteToMove;
}

int Board::getKingSquare(bool white) const
{
    return __builtin_ctzll(pieces[static_cast<int>(white ? Piece::WhiteKing : Piece::BlackKing)]);
}

void Board::undoMove()
{
    UndoInfo undo = history.back();
    history.pop_back();
    whiteToMove = !whiteToMove;

    whiteCastleKingSide = undo.whiteCastleKingSide;
    whiteCastleQueenSide = undo.whiteCastleQueenSide;
    blackCastleKingSide = undo.blackCastleKingSide;
    blackCastleQueenSide = undo.blackCastleQueenSide;

    enPassantSquare = undo.enPassantSquare;
    halfMoveClock = undo.halfMoveClock;
    fullMoveNumber = undo.fullMoveNumber;

    int to = undo.move.getTo();
    int from = undo.move.getFrom();
    MoveType mType = undo.move.getMoveType();

    if (mType == MoveType::Quiet)
    {
        addPiece(undo.movedPiece, from);
        removePiece(undo.movedPiece, to);
    }
    else if (mType == MoveType::Capture)
    {
        addPiece(undo.movedPiece, from);
        removePiece(undo.movedPiece, to);
        addPiece(undo.capturedPiece, to);
    }
    else if (mType == MoveType ::EnPassant)
    {
        addPiece(undo.movedPiece, from);
        removePiece(undo.movedPiece, to);
        int capturedSquare =
            whiteToMove ? to - 8 : to + 8;

        addPiece(undo.capturedPiece, capturedSquare);
    }
    else if (mType == MoveType ::Promotion)
    {
        addPiece(undo.movedPiece, from);
        removePiece(undo.move.getPromotionPiece(), to);
    }
    else if (mType == MoveType::PromotionCapture)
    {
        addPiece(undo.movedPiece, from);
        removePiece(undo.move.getPromotionPiece(), to);
        addPiece(undo.capturedPiece, to);
    }
    else if (mType == MoveType::KingCastle)
    {
        addPiece(undo.movedPiece, from);
        removePiece(undo.movedPiece, to);
        Piece rook = getPieceOnSquare(to - 1);
        removePiece(rook, to - 1);
        addPiece(rook, from + 3);
    }
    else if (mType == MoveType::QueenCastle)
    {
        addPiece(undo.movedPiece, from);
        removePiece(undo.movedPiece, to);
        Piece rook = getPieceOnSquare(to + 1);
        removePiece(rook, to + 1);
        addPiece(rook, from - 4);
    }
}