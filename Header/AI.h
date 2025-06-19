#ifndef AI_H
#define AI_H

#include "Game.h"
#include <utility>

class AI {
private:
    Player aiPlayer;
    
    // Core minimax algorithm with alpha-beta pruning
    int minimax(Game game, bool isMaximizing, int alpha, int beta, int depth = 0);
    
    // Strategic evaluation functions
    int evaluatePosition(const Game& game);
    int evaluateLines(const Game& game);

public:
    AI(Player aiPlayer);
    std::pair<int, int> findBestMove(Game game);
};

#endif