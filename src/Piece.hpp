#pragma once

#include "Chess.hpp"
#include <vector>
#include <string>
#include "Model3D.hpp"
#include <iostream>


class Piece {
public:
    Piece(PieceType type, PieceColor color, Position position)
        : m_type(type), m_color(color), m_position(position), m_hasMoved(false) {}
    
    virtual ~Piece() = default;
    

    virtual std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const = 0;
    
    // Getters
    PieceType getType() const { return m_type; }
    PieceColor getColor() const { return m_color; }
    Position getPosition() const { return m_position; }
    bool hasMoved() const { return m_hasMoved; }
    
    // Setters
    void setPosition(Position pos) { m_position = pos; m_hasMoved = true; }
    void setMoved(bool moved) { m_hasMoved = moved; }
    

    char getRepresentation() const;

    void render(glmax::Shader& shader);

protected:
    PieceType m_type;
    PieceColor m_color;
    Position m_position;
    bool m_hasMoved;

    Model3D m_model;
    

    std::vector<Position> getStraightMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const;
    std::vector<Position> getDiagonalMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const;
};


class Pawn : public Piece {
public:
    Pawn(PieceColor color, Position position)
        : Piece(PieceType::Pawn, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;

    void load_3D_model();

private:
    std::string _path = "pawn/pawn.obj";
    std::string _name = "pawn";

};

// Rook class
class Rook : public Piece {
public:
    Rook(PieceColor color, Position position)
        : Piece(PieceType::Rook, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;

    void load_3D_model();

private:
    std::string _path = "rook/rook.obj";
    std::string _name = "rook";

};


class Knight : public Piece {
public:
    Knight(PieceColor color, Position position)
        : Piece(PieceType::Knight, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;

    void load_3D_model();

private:
    std::string _path = "knight/knight.obj";
    std::string _name = "knight";

};


class Bishop : public Piece {
public:
    Bishop(PieceColor color, Position position)
        : Piece(PieceType::Bishop, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;

    void load_3D_model();

private:
    std::string _path = "bishop/bishop.obj";
    std::string _name = "bishop";

};


class Queen : public Piece {
public:
    Queen(PieceColor color, Position position)
        : Piece(PieceType::Queen, color, position) {}
    
    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;

    void load_3D_model();

private:
    std::string _path = "queen/queen.obj";
    std::string _name = "queen";

};


class King : public Piece {
public:
    King(PieceColor color, Position position)
        : Piece(PieceType::King, color, position) {}
    
    void load_3D_model();

    std::vector<Position> getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const override;

private:
    std::string _path = "king/king.obj";
    std::string _name = "king";

}; 