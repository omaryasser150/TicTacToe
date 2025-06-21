// Game.cpp placeholder
// Game.cpp
#include "Game.h"
#include <iostream>

// Constructor: starts a new game by calling reset
Game::Game() {
    reset();
}

// Resets the game to its starting state
void Game::reset() {
    // 3x3 board with all positions set to Player::NONE (empty)
    board = std::vector<std::vector<Player>>(3, std::vector<Player>(3, Player::NONE));
    
    // X starts the game
    currentPlayer = Player::X;

    // No moves played yet
    moveCount = 0;
}

// Attempts to place the current player's move at (row, col)
// Returns false if the cell is already taken
bool Game::makeMove(int row, int col) {
    if (board[row][col] != Player::NONE) return false; // Invalid move (cell taken)

    board[row][col] = currentPlayer;                   // Mark the cell
    currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X; // Switch player
    moveCount++;                                       // Increase move count
    return true;                                       // Move successful
}

// Checks if the player p has completed a winning row, column, or diagonal
bool Game::isWin(Player p) const {
    for (int i = 0; i < 3; ++i) {
        // Check row i or column i for all p
        if ((board[i][0] == p && board[i][1] == p && board[i][2] == p) ||
            (board[0][i] == p && board[1][i] == p && board[2][i] == p))
            return true;
    }

    // Check both diagonals
    if ((board[0][0] == p && board[1][1] == p && board[2][2] == p) ||
        (board[0][2] == p && board[1][1] == p && board[2][0] == p))
        return true;

    return false;
}

// Returns true if 9 moves have been played and no winner exists
bool Game::isDraw() const {
    return moveCount == 9 && getWinner() == Player::NONE;
}

// Returns which player's turn it is
Player Game::getCurrentPlayer() const {
    return currentPlayer;
}

// Returns Player::X or Player::O if they won; otherwise Player::NONE
Player Game::getWinner() const {
    for (auto p : { Player::X, Player::O }) {
        if (isWin(p)) return p;
    }
    return Player::NONE;
}

// Returns a list of all empty positions on the board
std::vector<std::pair<int, int>> Game::getAvailableMoves() const {
    std::vector<std::pair<int, int>> moves;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (board[r][c] == Player::NONE)
                moves.emplace_back(r, c);  // Add coordinate of empty cell
    return moves;
}

// Returns the player at the specified row and column
Player Game::at(int row, int col) const {
    return board[row][col];
}

// Prints the board to console
void Game::printBoard() const {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            char ch = (cell == Player::X) ? 'X' : (cell == Player::O) ? 'O' : '.';
            std::cout << ch << " ";
        }
        std::cout << "\n";
    }
}
