#include "Chess.hpp"
#include "Piece.hpp"
#include <imgui.h>
#include <iostream>

#include "lois/bernoulli.hpp"
#include "lois/binomial.hpp"
#include "lois/poisson.hpp"

Chess::Chess()
    : m_selectedPiece(false, Position{0, 0}),
      m_currentTurn(PieceColor::White),
      m_gameOver(false),
      m_exitGame(false),
      m_fullscreen(false),
      m_showPromotionModal(false),
      m_promotionPosition({0, 0}),
      m_showAboutDialog(false) {
    initBoard();
}

Chess::~Chess() = default;

void Chess::initBoard() {
    // Clear the board
    for (auto& row : m_board) {
        for (auto& cell : row) {
            cell = nullptr;
        }
    }
    
    // Setup pawns
    for (int x = 0; x < BOARD_SIZE; ++x) {
        m_board[1][x] = std::make_shared<Pawn>(PieceColor::Black, Position{x, 1});
        m_board[6][x] = std::make_shared<Pawn>(PieceColor::White, Position{x, 6});
    }
    
    // Setup rooks
    m_board[0][0] = std::make_shared<Rook>(PieceColor::Black, Position{0, 0});
    m_board[0][7] = std::make_shared<Rook>(PieceColor::Black, Position{7, 0});
    m_board[7][0] = std::make_shared<Rook>(PieceColor::White, Position{0, 7});
    m_board[7][7] = std::make_shared<Rook>(PieceColor::White, Position{7, 7});
    
    // Setup knights
    m_board[0][1] = std::make_shared<Knight>(PieceColor::Black, Position{1, 0});
    m_board[0][6] = std::make_shared<Knight>(PieceColor::Black, Position{6, 0});
    m_board[7][1] = std::make_shared<Knight>(PieceColor::White, Position{1, 7});
    m_board[7][6] = std::make_shared<Knight>(PieceColor::White, Position{6, 7});
    
    // Setup bishops
    m_board[0][2] = std::make_shared<Bishop>(PieceColor::Black, Position{2, 0});
    m_board[0][5] = std::make_shared<Bishop>(PieceColor::Black, Position{5, 0});
    m_board[7][2] = std::make_shared<Bishop>(PieceColor::White, Position{2, 7});
    m_board[7][5] = std::make_shared<Bishop>(PieceColor::White, Position{5, 7});
    
    // Setup queens
    m_board[0][3] = std::make_shared<Queen>(PieceColor::Black, Position{3, 0});
    m_board[7][3] = std::make_shared<Queen>(PieceColor::White, Position{3, 7});
    
    // Setup kings
    m_board[0][4] = std::make_shared<King>(PieceColor::Black, Position{4, 0});
    m_board[7][4] = std::make_shared<King>(PieceColor::White, Position{4, 7});
    
    // Initial values
    m_currentTurn = PieceColor::White;
    m_selectedPiece = {false, Position{0, 0}};
    m_gameOver = false;
    m_showPromotionModal = false;

    usernamePlayer1 = generateUsername();
    usernamePlayer2 = generateUsername();

    std::cout << "Username Player 1: " << usernamePlayer1 << std::endl;
    std::cout << "Username Player 2: " << usernamePlayer2 << std::endl;

}

void Chess::resetGame() {
    initBoard();
}

void Chess::drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Game")) {
            if (ImGui::MenuItem("New Game", "Ctrl+N")) {
                resetGame();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Toggle Fullscreen", "F11")) {
                toggleFullscreen();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                exitGame();
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                m_showAboutDialog = true;
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
    
    // Draw About dialog if open
    if (m_showAboutDialog) {
        drawAboutDialog();
    }
}

void Chess::drawAboutDialog() {
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Appearing);
    
    if (ImGui::Begin("About Chess", &m_showAboutDialog, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("Chess Game");
        ImGui::Separator();
        ImGui::Text("A 2D chess game implemented with ImGui");
        ImGui::Spacing();
        ImGui::Text("Features:");
        ImGui::BulletText("Standard chess pieces and movements");
        ImGui::BulletText("Two player gameplay");
        ImGui::BulletText("Pawn promotion");
        ImGui::BulletText("En passant");
        ImGui::Spacing();
        ImGui::Text("Controls:");
        ImGui::BulletText("Left click to select and move pieces");
        ImGui::BulletText("Right click to deselect a piece");
        ImGui::Spacing();
        
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 120);
        if (ImGui::Button("Close", ImVec2(100, 30))) {
            m_showAboutDialog = false;
        }
    }
    ImGui::End();
}

void Chess::draw() {
    // Draw the menu bar
    drawMenuBar();
    
    const float SQUARE_SIZE = 60.0f;
    
    // Set window flags based on fullscreen
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
    if (m_fullscreen) {
        // Fullscreen mode
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight())); // Position below menu bar
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));
        windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
    }
    
    ImGui::Begin("Chess", nullptr, windowFlags);
    
    // Show the current player's turn
    ImGui::Text("Current Turn: %s", m_currentTurn == PieceColor::White ? "White" : "Black");

    // Ajoutez ce code
    ImGui::Separator();
    ImGui::Text("Joueurs:");
    if (m_currentTurn == PieceColor::White) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "→ Blanc: %s", usernamePlayer1.c_str());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "  Noir: %s", usernamePlayer2.c_str());
    } else {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "  Blanc: %s", usernamePlayer1.c_str());
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "→ Noir: %s", usernamePlayer2.c_str());
    }
    ImGui::Separator();
    
    if (m_gameOver) {
        ImGui::Text("Game Over! %s wins!", m_currentTurn == PieceColor::White ? "Black" : "White");
        if (ImGui::Button("New Game")) {
            resetGame();
        }
    }
    
    // Create a child window for the chess board
    ImGui::BeginChild("ChessBoard", ImVec2(BOARD_SIZE * SQUARE_SIZE, BOARD_SIZE * SQUARE_SIZE), true);
    
    // Get the starting position of the child window
    ImVec2 boardPos = ImGui::GetCursorScreenPos();
    
    // Draw the chess board
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            // Calculate the position of the current square
            ImVec2 squarePos = ImVec2(boardPos.x + x * SQUARE_SIZE, boardPos.y + y * SQUARE_SIZE);
            
            // Calculate the color of the square (light or dark)
            ImVec4 squareColor;
            bool isDarkSquare = (x + y) % 2 != 0;
            
            if (isDarkSquare) {
                squareColor = ImVec4(0.5f, 0.3f, 0.0f, 1.0f); // Dark brown
            } else {
                squareColor = ImVec4(0.9f, 0.8f, 0.6f, 1.0f); // Light brown
            }
            
            // Check if the square is a valid move for the selected piece
            bool isValidMove = false;
            if (m_selectedPiece.first) {
                Position selectedPos = m_selectedPiece.second;
                Position currentPos = {x, y};
                
                // Get possible moves for the selected piece
                std::vector<Position> possibleMoves = getPossibleMoves(selectedPos);
                
                // Check if the current position is in the possible moves
                isValidMove = std::find(possibleMoves.begin(), possibleMoves.end(), currentPos) != possibleMoves.end();
            }
            
            // Change color if it's a valid move
            if (isValidMove) {
                squareColor.x = 0.6f;
                squareColor.y = 0.8f;
                squareColor.z = 0.6f;
            }
            
            // Change color if it's the selected piece
            if (m_selectedPiece.first && m_selectedPiece.second.x == x && m_selectedPiece.second.y == y) {
                squareColor.x = 0.8f;
                squareColor.y = 0.8f;
                squareColor.z = 0.4f;
            }
            
            // Draw the square
            ImGui::GetWindowDrawList()->AddRectFilled(
                squarePos,
                ImVec2(squarePos.x + SQUARE_SIZE, squarePos.y + SQUARE_SIZE),
                ImGui::ColorConvertFloat4ToU32(squareColor)
            );
            
            // Draw the piece if there is one
            if (m_board[y][x]) {
                PieceColor pieceColor = m_board[y][x]->getColor();
                char pieceChar = m_board[y][x]->getRepresentation();
                
                // Change the color of the button based on the piece color
                ImGui::PushStyleColor(ImGuiCol_Button, 
                    pieceColor == PieceColor::White ? 
                    ImVec4(0.9f, 0.9f, 0.9f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 
                    pieceColor == PieceColor::White ? 
                    ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, 
                    pieceColor == PieceColor::White ? 
                    ImVec4(0.0f, 0.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                
                // Position the button at the current square
                ImGui::SetCursorScreenPos(squarePos);
                
                // Create a unique ID for the button
                ImGui::PushID(y * BOARD_SIZE + x);
                
                if (ImGui::Button(&pieceChar, ImVec2(SQUARE_SIZE, SQUARE_SIZE))) {
                    handleBoardClick(x, y);
                }
                
                ImGui::PopID();
                ImGui::PopStyleColor(3);
            } else {
                // Draw an invisible button on empty squares to handle clicks
                ImGui::SetCursorScreenPos(squarePos);
                ImGui::PushID(y * BOARD_SIZE + x);
                
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                
                if (ImGui::Button("##empty", ImVec2(SQUARE_SIZE, SQUARE_SIZE))) {
                    handleBoardClick(x, y);
                }
                
                ImGui::PopStyleColor(2);
                ImGui::PopID();
            }
        }
    }
    
    // Check for right-click to deselect piece
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        handleRightClick();
    }
    
    ImGui::EndChild();
    
    // Draw promotion modal if needed
    if (m_showPromotionModal) {
        drawPromotionModal();
    }
    
    ImGui::End();
}

void Chess::handleBoardClick(int x, int y) {
    // Skip if game is over
    if (m_gameOver) {
        return;
    }
    
    Position clickedPos = {x, y};
    
    // If no piece is selected, select a piece
    if (!m_selectedPiece.first) {
        // Check if there is a piece at the clicked position
        if (m_board[y][x]) {
            // Check if the piece belongs to the current player
            if (m_board[y][x]->getColor() == m_currentTurn) {
                m_selectedPiece = {true, clickedPos};
            }
        }
    }
    // If a piece is already selected, try to move it
    else {
        Position selectedPos = m_selectedPiece.second;
        
        // Skip if clicking on the same position
        if (selectedPos.x == x && selectedPos.y == y) {
            return;
        }
        
        // Check if the move is valid
        if (isValidMove(selectedPos, clickedPos)) {
            // Move the piece
            movePiece(selectedPos, clickedPos);
            
            // Deselect the piece
            m_selectedPiece = {false, Position{0, 0}};
        }
        // If the clicked position has a piece of the current player, select it instead
        else if (m_board[y][x] && m_board[y][x]->getColor() == m_currentTurn) {
            m_selectedPiece = {true, clickedPos};
        }
    }
}

void Chess::handleRightClick() {
    // Deselect the current piece
    m_selectedPiece = {false, Position{0, 0}};
}

std::vector<Position> Chess::getPossibleMoves(const Position& pos) const {
    // Check if the position is valid and has a piece
    if (!isValidPosition(pos) || !m_board[pos.y][pos.x]) {
        return {};
    }
    
    // Get the basic moves for the piece
    std::vector<Position> moves = m_board[pos.y][pos.x]->getPossibleMoves(m_board);
    
    // Handle special cases like en passant
    if (m_board[pos.y][pos.x]->getType() == PieceType::Pawn) {
        // Check for en passant
        PieceColor color = m_board[pos.y][pos.x]->getColor();
        int direction = (color == PieceColor::White) ? -1 : 1;
        
        // If last moved piece was a pawn and it moved two steps
        if (m_lastMovedPiece.second && m_lastMovedPiece.second->getType() == PieceType::Pawn) {
            Position lastPos = m_lastMovedPiece.first;
            Position curPos = m_lastMovedPiece.second->getPosition();
            
            // Check if it moved two steps
            if (std::abs(curPos.y - lastPos.y) == 2) {
                // Check if it's adjacent to our pawn
                if (std::abs(curPos.x - pos.x) == 1 && curPos.y == pos.y) {
                    // Add the en passant move
                    Position enPassantPos = {curPos.x, pos.y + direction};
                    moves.push_back(enPassantPos);
                }
            }
        }
    }
    
    return moves;
}

bool Chess::isValidMove(const Position& from, const Position& to) const {
    // Check if positions are valid
    if (!isValidPosition(from) || !isValidPosition(to)) {
        return false;
    }
    
    // Check if there is a piece at the from position
    if (!m_board[from.y][from.x]) {
        return false;
    }
    
    // Check if the piece belongs to the current player
    if (m_board[from.y][from.x]->getColor() != m_currentTurn) {
        return false;
    }
    
    // Get possible moves for the piece
    std::vector<Position> possibleMoves = getPossibleMoves(from);
    
    // Check if the to position is in the possible moves
    return std::find(possibleMoves.begin(), possibleMoves.end(), to) != possibleMoves.end();
}

void Chess::movePiece(const Position& from, const Position& to) {
    // Check if the move is valid
    if (!isValidMove(from, to)) {
        return;
    }
    
    // Store the last moved piece and its previous position
    m_lastMovedPiece = {from, m_board[from.y][from.x]};
    
    // Check for en passant
    if (m_board[from.y][from.x]->getType() == PieceType::Pawn) {
        // If moving diagonally to an empty square, it's en passant
        if (from.x != to.x && !m_board[to.y][to.x]) {
            // Capture the pawn that moved two steps
            m_board[from.y][to.x] = nullptr;
        }
    }
    
    // Capture piece if there is one at the destination
    if (m_board[to.y][to.x]) {
        // Check if king is captured (game over)
        if (m_board[to.y][to.x]->getType() == PieceType::King) {
            m_gameOver = true;
        }
    }
    
    // Update piece position
    m_board[from.y][from.x]->setPosition(to);
    
    // Move piece to the new position
    m_board[to.y][to.x] = m_board[from.y][from.x];
    m_board[from.y][from.x] = nullptr;
    
    // Check for pawn promotion
    checkForPromotion(to);
    
    // If not showing the promotion modal, switch the turn
    if (!m_showPromotionModal) {
        switchTurn();
    }
}

void Chess::switchTurn() {
    m_currentTurn = (m_currentTurn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}

bool Chess::isGameOver() const {
    return m_gameOver;
}

void Chess::checkForPromotion(const Position& pos) {
    // Check if the piece is a pawn and it reached the opposite end
    if (m_board[pos.y][pos.x]->getType() == PieceType::Pawn) {
        PieceColor color = m_board[pos.y][pos.x]->getColor();
        
        if ((color == PieceColor::White && pos.y == 0) || 
            (color == PieceColor::Black && pos.y == 7)) {
            // Set promotion data
            m_showPromotionModal = true;
            m_promotionPosition = pos;
        }
    }
}

void Chess::promotePawn(const Position& pos, PieceType type) {
    // Check if the position is valid
    if (!isValidPosition(pos) || !m_board[pos.y][pos.x]) {
        return;
    }
    
    // Get the color of the pawn
    PieceColor color = m_board[pos.y][pos.x]->getColor();
    
    // Replace the pawn with the new piece
    switch (type) {
        case PieceType::Queen:
            m_board[pos.y][pos.x] = std::make_shared<Queen>(color, pos);
            break;
        case PieceType::Rook:
            m_board[pos.y][pos.x] = std::make_shared<Rook>(color, pos);
            break;
        case PieceType::Bishop:
            m_board[pos.y][pos.x] = std::make_shared<Bishop>(color, pos);
            break;
        case PieceType::Knight:
            m_board[pos.y][pos.x] = std::make_shared<Knight>(color, pos);
            break;
        default:
            // Default to queen
            m_board[pos.y][pos.x] = std::make_shared<Queen>(color, pos);
            break;
    }
    
    // Mark that the piece has moved
    m_board[pos.y][pos.x]->setMoved(true);
    
    // Close the modal
    m_showPromotionModal = false;
    
    // Switch turn
    switchTurn();
}

void Chess::drawPromotionModal() {
    // Set the modal window position to the center of the screen
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    if (ImGui::BeginPopupModal("Pawn Promotion", &m_showPromotionModal, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Choose a piece to promote to:");
        
        // Get the color of the pawn
        PieceColor color = m_board[m_promotionPosition.y][m_promotionPosition.x]->getColor();
        
        // Promotion options
        ImGui::PushStyleColor(ImGuiCol_Button, 
            color == PieceColor::White ? 
            ImVec4(0.9f, 0.9f, 0.9f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 
            color == PieceColor::White ? 
            ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, 
            color == PieceColor::White ? 
            ImVec4(0.0f, 0.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        char queenChar = 'Q';
        char rookChar = 'R';
        char bishopChar = 'B';
        char knightChar = 'N';
        
        // Queen
        if (ImGui::Button(&queenChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Queen);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        
        // Rook
        if (ImGui::Button(&rookChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Rook);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        
        // Bishop
        if (ImGui::Button(&bishopChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Bishop);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        
        // Knight
        if (ImGui::Button(&knightChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Knight);
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::PopStyleColor(3);
        
        ImGui::EndPopup();
    } else {
        // If the modal isn't open, open it
        ImGui::OpenPopup("Pawn Promotion");
    }
}

std::string Chess::generateUsername() {
    std::string playerName = "";
    
    bool hasTitlePrefix = bernoulliTitleAttribution(0.7);
    std::string nickname = playerName;
    
    if (hasTitlePrefix) {
        nickname = binomialNicknameGenerator(3, 0.6) + " " + playerName;
    }
    
    nickname = poissonSpecialCharacters(1.5, nickname);
    
    return nickname;
}