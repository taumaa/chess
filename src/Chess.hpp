#pragma once

#include <array>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "imgui.h"
#include "Model3D.hpp"

class Piece;


struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};


enum class PieceType {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};


enum class PieceColor {
    White,
    Black
};


class Chess {
public:
    Chess();
    ~Chess();
    

    void initBoard();
    

    void draw();


    void handleBoardClick(int x, int y);
    void handleRightClick();
    

    std::vector<Position> getPossibleMoves(const Position& pos) const;
    

    bool isValidMove(const Position& from, const Position& to) const;
    

    void movePiece(const Position& from, const Position& to);
    

    bool isGameOver() const;
    

    PieceColor getCurrentTurn() const { return m_currentTurn; }
    

    void resetGame();
    void exitGame() { m_exitGame = true; }
    bool shouldExitGame() const { return m_exitGame; }
    void toggleFullscreen() { m_fullscreen = !m_fullscreen; }
    bool isFullscreen() const { return m_fullscreen; }
    
    // Draw menu bar
    void drawMenuBar();

    void load_board_3D();
    void render(glmax::Shader& shader);
    void load_pieces_3D();



private:
    static constexpr int BOARD_SIZE = 8;
    

    std::array<std::array<std::shared_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> m_board;
    

    std::pair<bool, Position> m_selectedPiece;
    

    PieceColor m_currentTurn;
    

    std::pair<Position, std::shared_ptr<Piece>> m_lastMovedPiece;
    

    bool m_gameOver;
    bool m_exitGame;
    bool m_fullscreen;
    

    bool m_showPromotionModal;
    Position m_promotionPosition;
    

    void switchTurn();
    void checkForPromotion(const Position& pos);
    void promotePawn(const Position& pos, PieceType type);
    

    bool isValidPosition(const Position& pos) const {
        return pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE;
    }
    

    void drawPromotionModal();

    std::string generateUsername();

    std::string usernamePlayer1;
    std::string usernamePlayer2;

    std::pair<ImVec4, ImVec4> m_chessboardColors;

    Model3D _board3D;

    std::string _path = "board/board.obj";
    std::string _name = "board";
}; 


