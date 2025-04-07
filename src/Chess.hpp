#pragma once

#include <array>
#include <vector>
#include <memory>
#include <string>
#include <utility>

// Forward declaration
class Piece;

// Position on the chess board (0-7, 0-7)
struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Type of chess pieces
enum class PieceType {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

// Color of chess pieces
enum class PieceColor {
    White,
    Black
};

// Chess class to manage the game
class Chess {
public:
    Chess();
    ~Chess();
    
    // Initialize the chess board
    void initBoard();
    
    // Draw the chess board using ImGui
    void draw();

    // Handle selecting and moving pieces
    void handleBoardClick(int x, int y);
    void handleRightClick();
    
    // Return possible moves for a piece
    std::vector<Position> getPossibleMoves(const Position& pos) const;
    
    // Check if a move is valid
    bool isValidMove(const Position& from, const Position& to) const;
    
    // Move a piece
    void movePiece(const Position& from, const Position& to);
    
    // Check if game is over
    bool isGameOver() const;
    
    // Get the current player turn
    PieceColor getCurrentTurn() const { return m_currentTurn; }
    
    // Menu actions
    void resetGame();
    void exitGame() { m_exitGame = true; }
    bool shouldExitGame() const { return m_exitGame; }
    void toggleFullscreen() { m_fullscreen = !m_fullscreen; }
    bool isFullscreen() const { return m_fullscreen; }
    
    // Draw menu bar
    void drawMenuBar();



private:
    static constexpr int BOARD_SIZE = 8;
    
    // Chess board (8x8 grid of pieces, nullptr if empty)
    std::array<std::array<std::shared_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> m_board;
    
    // Currently selected piece position
    std::pair<bool, Position> m_selectedPiece;
    
    // Current player's turn
    PieceColor m_currentTurn;
    
    // Last moved piece (for en passant rule)
    std::pair<Position, std::shared_ptr<Piece>> m_lastMovedPiece;
    
    // Game state flags
    bool m_gameOver;
    bool m_exitGame;
    bool m_fullscreen;
    
    // Open promotion modal flag and position
    bool m_showPromotionModal;
    Position m_promotionPosition;
    
    // About dialog
    bool m_showAboutDialog;
    
    // Helper functions
    void switchTurn();
    void checkForPromotion(const Position& pos);
    void promotePawn(const Position& pos, PieceType type);
    
    // Check if a position is on the board
    bool isValidPosition(const Position& pos) const {
        return pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE;
    }
    
    // Draw promotion modal
    void drawPromotionModal();
    
    // Draw about dialog
    void drawAboutDialog();

    std::string generateUsername();

    std::string usernamePlayer1;
    std::string usernamePlayer2;
}; 