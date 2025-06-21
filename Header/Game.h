// Game.h placeholder// Game.h
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

// Enum representing the three possible states for a board cell or player
enum class Player { NONE = 0, X, O };

// The Game class handles the board, move logic, win/draw detection, etc.
class Game {
private:
    // 3x3 board represented as a 2D vector of Player enum values
    std::vector<std::vector<Player>> board;

    // Tracks whose turn it is
    Player currentPlayer;

    // Number of moves played so far (used for detecting draw)
    int moveCount;

public:
    Game();                              // Constructor initializes the game
    void reset();                        // Resets the board and turn to initial state
    bool makeMove(int row, int col);     // Attempts to place currentPlayer's move
    bool isWin(Player p) const;          // Returns true if player p has won
    bool isDraw() const;                 // Returns true if the board is full and no winner
    Player getCurrentPlayer() const;     // Returns current player's turn
    Player getWinner() const;            // Returns the winning player, or NONE
    std::vector<std::pair<int, int>> getAvailableMoves() const;  // Returns list of empty cells
    void printBoard() const;             // Prints board to console
    Player at(int row, int col) const;   // Gets the player at a given board position
};

#endif
