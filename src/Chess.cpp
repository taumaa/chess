#include "Chess.hpp"
#include "Piece.hpp"
#include <imgui.h>
#include <iostream>

#include "lois/bernoulli.hpp"
#include "lois/binomial.hpp"
#include "lois/poisson.hpp"
#include "lois/normale.hpp"

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

    for (auto& row : m_board) {
        for (auto& cell : row) {
            cell = nullptr;
        }
    }
    

    for (int x = 0; x < BOARD_SIZE; ++x) {
        m_board[1][x] = std::make_shared<Pawn>(PieceColor::Black, Position{x, 1});
        m_board[6][x] = std::make_shared<Pawn>(PieceColor::White, Position{x, 6});
    }
    

    m_board[0][0] = std::make_shared<Rook>(PieceColor::Black, Position{0, 0});
    m_board[0][7] = std::make_shared<Rook>(PieceColor::Black, Position{7, 0});
    m_board[7][0] = std::make_shared<Rook>(PieceColor::White, Position{0, 7});
    m_board[7][7] = std::make_shared<Rook>(PieceColor::White, Position{7, 7});
    

    m_board[0][1] = std::make_shared<Knight>(PieceColor::Black, Position{1, 0});
    m_board[0][6] = std::make_shared<Knight>(PieceColor::Black, Position{6, 0});
    m_board[7][1] = std::make_shared<Knight>(PieceColor::White, Position{1, 7});
    m_board[7][6] = std::make_shared<Knight>(PieceColor::White, Position{6, 7});
    

    m_board[0][2] = std::make_shared<Bishop>(PieceColor::Black, Position{2, 0});
    m_board[0][5] = std::make_shared<Bishop>(PieceColor::Black, Position{5, 0});
    m_board[7][2] = std::make_shared<Bishop>(PieceColor::White, Position{2, 7});
    m_board[7][5] = std::make_shared<Bishop>(PieceColor::White, Position{5, 7});
    

    m_board[0][3] = std::make_shared<Queen>(PieceColor::Black, Position{3, 0});
    m_board[7][3] = std::make_shared<Queen>(PieceColor::White, Position{3, 7});
    

    m_board[0][4] = std::make_shared<King>(PieceColor::Black, Position{4, 0});
    m_board[7][4] = std::make_shared<King>(PieceColor::White, Position{4, 7});
    

    m_currentTurn = PieceColor::White;
    m_selectedPiece = {false, Position{0, 0}};
    m_gameOver = false;
    m_showPromotionModal = false;

    usernamePlayer1 = generateUsername();
    usernamePlayer2 = generateUsername();

    m_chessboardColors = normaleChessboardColors(0.01f);

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
    

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
    if (m_fullscreen) {

        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight())); // Position below menu bar
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));
        windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
    }
    
    ImGui::Begin("Chess", nullptr, windowFlags);
    

    ImGui::Text("Current Turn: %s", m_currentTurn == PieceColor::White ? "White" : "Black");


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
    

    ImGui::BeginChild("ChessBoard", ImVec2(BOARD_SIZE * SQUARE_SIZE, BOARD_SIZE * SQUARE_SIZE), true);
    

    ImVec2 boardPos = ImGui::GetCursorScreenPos();
    

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {

            ImVec2 squarePos = ImVec2(boardPos.x + x * SQUARE_SIZE, boardPos.y + y * SQUARE_SIZE);
            

            ImVec4 squareColor;
            bool isDarkSquare = (x + y) % 2 != 0;
            
            if (isDarkSquare) {
                squareColor = m_chessboardColors.second;
            } else {
                squareColor = m_chessboardColors.first;
            }
            

            bool isValidMove = false;
            if (m_selectedPiece.first) {
                Position selectedPos = m_selectedPiece.second;
                Position currentPos = {x, y};
                

                std::vector<Position> possibleMoves = getPossibleMoves(selectedPos);
                

                isValidMove = std::find(possibleMoves.begin(), possibleMoves.end(), currentPos) != possibleMoves.end();
            }
            

            if (isValidMove) {
                squareColor.x = 0.6f;
                squareColor.y = 0.8f;
                squareColor.z = 0.6f;
            }
            

            if (m_selectedPiece.first && m_selectedPiece.second.x == x && m_selectedPiece.second.y == y) {
                squareColor.x = 0.8f;
                squareColor.y = 0.8f;
                squareColor.z = 0.4f;
            }
            

            ImGui::GetWindowDrawList()->AddRectFilled(
                squarePos,
                ImVec2(squarePos.x + SQUARE_SIZE, squarePos.y + SQUARE_SIZE),
                ImGui::ColorConvertFloat4ToU32(squareColor)
            );
            

            if (m_board[y][x]) {
                PieceColor pieceColor = m_board[y][x]->getColor();
                char pieceChar = m_board[y][x]->getRepresentation();
                

                ImGui::PushStyleColor(ImGuiCol_Button, 
                    pieceColor == PieceColor::White ? 
                    ImVec4(0.9f, 0.9f, 0.9f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 
                    pieceColor == PieceColor::White ? 
                    ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, 
                    pieceColor == PieceColor::White ? 
                    ImVec4(0.0f, 0.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                

                ImGui::SetCursorScreenPos(squarePos);
                

                ImGui::PushID(y * BOARD_SIZE + x);
                
                if (ImGui::Button(&pieceChar, ImVec2(SQUARE_SIZE, SQUARE_SIZE))) {
                    handleBoardClick(x, y);
                }
                
                ImGui::PopID();
                ImGui::PopStyleColor(3);
            } else {

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
    

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        handleRightClick();
    }
    
    ImGui::EndChild();
    

    if (m_showPromotionModal) {
        drawPromotionModal();
    }
    
    ImGui::End();
}

void Chess::handleBoardClick(int x, int y) {

    if (m_gameOver) {
        return;
    }
    
    Position clickedPos = {x, y};
    

    if (!m_selectedPiece.first) {

        if (m_board[y][x]) {

            if (m_board[y][x]->getColor() == m_currentTurn) {
                m_selectedPiece = {true, clickedPos};
            }
        }
    }

    else {
        Position selectedPos = m_selectedPiece.second;
        

        if (selectedPos.x == x && selectedPos.y == y) {
            return;
        }
        

        if (isValidMove(selectedPos, clickedPos)) {

            movePiece(selectedPos, clickedPos);
            

            m_selectedPiece = {false, Position{0, 0}};
        }

        else if (m_board[y][x] && m_board[y][x]->getColor() == m_currentTurn) {
            m_selectedPiece = {true, clickedPos};
        }
    }
}

void Chess::handleRightClick() {

    m_selectedPiece = {false, Position{0, 0}};
}

std::vector<Position> Chess::getPossibleMoves(const Position& pos) const {

    if (!isValidPosition(pos) || !m_board[pos.y][pos.x]) {
        return {};
    }
    
    std::vector<Position> moves = m_board[pos.y][pos.x]->getPossibleMoves(m_board);
    

    if (m_board[pos.y][pos.x]->getType() == PieceType::Pawn) {

        PieceColor color = m_board[pos.y][pos.x]->getColor();
        int direction = (color == PieceColor::White) ? -1 : 1;
        

        if (m_lastMovedPiece.second && m_lastMovedPiece.second->getType() == PieceType::Pawn) {
            Position lastPos = m_lastMovedPiece.first;
            Position curPos = m_lastMovedPiece.second->getPosition();
            

            if (std::abs(curPos.y - lastPos.y) == 2) {

                if (std::abs(curPos.x - pos.x) == 1 && curPos.y == pos.y) {

                    Position enPassantPos = {curPos.x, pos.y + direction};
                    moves.push_back(enPassantPos);
                }
            }
        }
    }
    
    return moves;
}

bool Chess::isValidMove(const Position& from, const Position& to) const {

    if (!isValidPosition(from) || !isValidPosition(to)) {
        return false;
    }
    

    if (!m_board[from.y][from.x]) {
        return false;
    }
    

    if (m_board[from.y][from.x]->getColor() != m_currentTurn) {
        return false;
    }
    

    std::vector<Position> possibleMoves = getPossibleMoves(from);
    

    return std::find(possibleMoves.begin(), possibleMoves.end(), to) != possibleMoves.end();
}

void Chess::movePiece(const Position& from, const Position& to) {
    // Check if the move is valid
    if (!isValidMove(from, to)) {
        return;
    }
    

    m_lastMovedPiece = {from, m_board[from.y][from.x]};
    

    if (m_board[from.y][from.x]->getType() == PieceType::Pawn) {

        if (from.x != to.x && !m_board[to.y][to.x]) {

            m_board[from.y][to.x] = nullptr;
        }
    }
    

    if (m_board[to.y][to.x]) {

        if (m_board[to.y][to.x]->getType() == PieceType::King) {
            m_gameOver = true;
        }
    }
    

    m_board[from.y][from.x]->setPosition(to);


    m_board[to.y][to.x] = m_board[from.y][from.x];
    m_board[from.y][from.x] = nullptr;
    

    checkForPromotion(to);
    

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

    if (!isValidPosition(pos) || !m_board[pos.y][pos.x]) {
        return;
    }
    

    PieceColor color = m_board[pos.y][pos.x]->getColor();
    

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
    

    m_board[pos.y][pos.x]->setMoved(true);
    

    m_showPromotionModal = false;
    

    switchTurn();
}

void Chess::drawPromotionModal() {

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    if (ImGui::BeginPopupModal("Pawn Promotion", &m_showPromotionModal, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Choose a piece to promote to:");
        

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
        

        if (ImGui::Button(&queenChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Queen);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        

        if (ImGui::Button(&rookChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Rook);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();


        if (ImGui::Button(&bishopChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Bishop);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        

        if (ImGui::Button(&knightChar, ImVec2(60.0f, 60.0f))) {
            promotePawn(m_promotionPosition, PieceType::Knight);
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::PopStyleColor(3);
        
        ImGui::EndPopup();
    } else {

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