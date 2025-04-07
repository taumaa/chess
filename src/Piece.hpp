#pragma once

#include "Chess.hpp"
#include <vector>
#include <string>

// Base class for all chess pieces
class Piece {
public:
    Piece(PieceType type, PieceColor color, Position position)
        : m_type(type), m_color(color), m_position(position), m_hasMoved(false) {}
    
    virtual ~Piece() = default;
    
    // Get possible moves for this piece
    virtual std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const = 0;
    
    // Getters
    PieceType getType() const { return m_type; }
    PieceColor getColor() const { return m_color; }
    Position getPosition() const { return m_position; }
    bool hasMoved() const { return m_hasMoved; }
    
    // Setters
    void setPosition(Position pos) { m_position = pos; m_hasMoved = true; }
    void setMoved(bool moved) { m_hasMoved = moved; }
    
    // Get piece representation letter
    char getRepresentation() const;

protected:
    PieceType m_type;
    PieceColor m_color;
    Position m_position;
    bool m_hasMoved;
    
    // Helper methods for move generation
    std::vector<Position> getStraightMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const;
    std::vector<Position> getDiagonalMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const;
};

// Pawn class
class Pawn : public Piece {
public:
    Pawn(PieceColor color, Position position)
        : Piece(PieceType::Pawn, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;
};

// Rook class
class Rook : public Piece {
public:
    Rook(PieceColor color, Position position)
        : Piece(PieceType::Rook, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;
};

// Knight class
class Knight : public Piece {
public:
    Knight(PieceColor color, Position position)
        : Piece(PieceType::Knight, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;
};

// Bishop class
class Bishop : public Piece {
public:
    Bishop(PieceColor color, Position position)
        : Piece(PieceType::Bishop, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;
};

// Queen class
class Queen : public Piece {
public:
    Queen(PieceColor color, Position position)
        : Piece(PieceType::Queen, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;
};

// King class
class King : public Piece {
public:
    King(PieceColor color, Position position)
        : Piece(PieceType::King, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;
}; 