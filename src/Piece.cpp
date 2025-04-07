#include "Piece.hpp"
#include <algorithm>

char Piece::getRepresentation() const {
    char representation;
    
    switch (m_type) {
        case PieceType::Pawn:
            representation = 'P';
            break;
        case PieceType::Rook:
            representation = 'R';
            break;
        case PieceType::Knight:
            representation = 'N';
            break;
        case PieceType::Bishop:
            representation = 'B';
            break;
        case PieceType::Queen:
            representation = 'Q';
            break;
        case PieceType::King:
            representation = 'K';
            break;
        default:
            representation = '?';
    }
    
    return representation;
}

std::vector<Position> Piece::getStraightMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {
    std::vector<Position> moves;


    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};
    
    for (int dir = 0; dir < 4; ++dir) {
        for (int step = 1; step < 8; ++step) {
            Position newPos = {m_position.x + dx[dir] * step, m_position.y + dy[dir] * step};
            

            if (newPos.x < 0 || newPos.x >= 8 || newPos.y < 0 || newPos.y >= 8) {
                break;
            }
            
            // Check if position is empty
            if (!board[newPos.y][newPos.x]) {
                moves.push_back(newPos);
            }

            else if (board[newPos.y][newPos.x]->getColor() != m_color) {
                moves.push_back(newPos);
                break;
            }

            else {
                break;
            }
        }
    }
    
    return moves;
}

std::vector<Position> Piece::getDiagonalMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {
    std::vector<Position> moves;
    

    const int dx[] = {1, 1, -1, -1};
    const int dy[] = {-1, 1, 1, -1};
    
    for (int dir = 0; dir < 4; ++dir) {
        for (int step = 1; step < 8; ++step) {
            Position newPos = {m_position.x + dx[dir] * step, m_position.y + dy[dir] * step};
            

            if (newPos.x < 0 || newPos.x >= 8 || newPos.y < 0 || newPos.y >= 8) {
                break;
            }
            

            if (!board[newPos.y][newPos.x]) {
                moves.push_back(newPos);
            }

            else if (board[newPos.y][newPos.x]->getColor() != m_color) {
                moves.push_back(newPos);
                break;
            }

            else {
                break;
            }
        }
    }
    
    return moves;
}

std::vector<Position> Pawn::getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {
    std::vector<Position> moves;
    

    const int direction = (m_color == PieceColor::White) ? -1 : 1;
    

    Position oneStep = {m_position.x, m_position.y + direction};
    if (oneStep.y >= 0 && oneStep.y < 8 && !board[oneStep.y][oneStep.x]) {
        moves.push_back(oneStep);
        

        if (!m_hasMoved) {
            Position twoStep = {m_position.x, m_position.y + 2 * direction};
            if (twoStep.y >= 0 && twoStep.y < 8 && !board[twoStep.y][twoStep.x]) {
                moves.push_back(twoStep);
            }
        }
    }
    

    for (int dx = -1; dx <= 1; dx += 2) {
        Position capturePos = {m_position.x + dx, m_position.y + direction};
        
        if (capturePos.x >= 0 && capturePos.x < 8 && capturePos.y >= 0 && capturePos.y < 8) {

            if (board[capturePos.y][capturePos.x] && board[capturePos.y][capturePos.x]->getColor() != m_color) {
                moves.push_back(capturePos);
            }
            

        }
    }

    
    return moves;
}

std::vector<Position> Rook::getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {
    return getStraightMoves(board);
}

std::vector<Position> Knight::getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {
    std::vector<Position> moves;
    
    const int knightMoves[][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (const auto& move : knightMoves) {
        Position newPos = {m_position.x + move[0], m_position.y + move[1]};
        
        // Check if position is valid
        if (newPos.x < 0 || newPos.x >= 8 || newPos.y < 0 || newPos.y >= 8) {
            continue;
        }
        
        // Check if position is empty or has an enemy piece
        if (!board[newPos.y][newPos.x] || board[newPos.y][newPos.x]->getColor() != m_color) {
            moves.push_back(newPos);
        }
    }
    
    return moves;
}

std::vector<Position> Bishop::getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {
    return getDiagonalMoves(board);
}

std::vector<Position> Queen::getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {

    std::vector<Position> straightMoves = getStraightMoves(board);
    std::vector<Position> diagonalMoves = getDiagonalMoves(board);
    
    // Combine the moves
    std::vector<Position> allMoves = straightMoves;
    allMoves.insert(allMoves.end(), diagonalMoves.begin(), diagonalMoves.end());
    
    return allMoves;
}

std::vector<Position> King::getPossibleMoves(const std::array<std::array<std::shared_ptr<Piece>, 8>, 8>& board) const {
    std::vector<Position> moves;
    
    const int kingMoves[][2] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1, 0},            {1, 0},
        {-1, 1},  {0, 1},  {1, 1}
    };
    
    for (const auto& move : kingMoves) {
        Position newPos = {m_position.x + move[0], m_position.y + move[1]};
        

        if (newPos.x < 0 || newPos.x >= 8 || newPos.y < 0 || newPos.y >= 8) {
            continue;
        }
        

        if (!board[newPos.y][newPos.x] || board[newPos.y][newPos.x]->getColor() != m_color) {
            moves.push_back(newPos);
        }
    }

    
    return moves;
} 