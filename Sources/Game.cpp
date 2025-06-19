// Game.cpp placeholder
// Game.cpp
#include "Game.h"
#include <iostream>

Game::Game() {
    reset();
}

void Game::reset() {
    board = std::vector<std::vector<Player>>(3, std::vector<Player>(3, Player::NONE));
    currentPlayer = Player::X;
    moveCount = 0;
}

bool Game::makeMove(int row, int col) {
    if (board[row][col] != Player::NONE) return false;
    board[row][col] = currentPlayer;
    currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
    moveCount++;
    return true;
}

bool Game::isWin(Player p) const {
    for (int i = 0; i < 3; ++i) {
        if ((board[i][0] == p && board[i][1] == p && board[i][2] == p) ||
            (board[0][i] == p && board[1][i] == p && board[2][i] == p))
            return true;
    }
    if ((board[0][0] == p && board[1][1] == p && board[2][2] == p) ||
        (board[0][2] == p && board[1][1] == p && board[2][0] == p))
        return true;
    return false;
}

bool Game::isDraw() const {
    return moveCount == 9 && getWinner() == Player::NONE;
}

Player Game::getCurrentPlayer() const {
    return currentPlayer;
}

Player Game::getWinner() const {
    for (auto p : { Player::X, Player::O }) {
        if (isWin(p)) return p;
    }
    return Player::NONE;
}

std::vector<std::pair<int, int>> Game::getAvailableMoves() const {
    std::vector<std::pair<int, int>> moves;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (board[r][c] == Player::NONE)
                moves.emplace_back(r, c);
    return moves;
}

Player Game::at(int row, int col) const {
    return board[row][col];
}

void Game::printBoard() const {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            char ch = (cell == Player::X) ? 'X' : (cell == Player::O) ? 'O' : '.';
            std::cout << ch << " ";
        }
        std::cout << "\n";
    }
}
