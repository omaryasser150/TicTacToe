// Game.h placeholder// Game.h
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

enum class Player { NONE = 0, X, O };

class Game {
private:
    std::vector<std::vector<Player>> board;
    Player currentPlayer;
    int moveCount;

public:
    Game();
    void reset();
    bool makeMove(int row, int col);
    bool isWin(Player p) const;
    bool isDraw() const;
    Player getCurrentPlayer() const;
    Player getWinner() const;
    std::vector<std::pair<int, int>> getAvailableMoves() const;
    void printBoard() const;
    Player at(int row, int col) const;
};

#endif
