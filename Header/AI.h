// AI.h placeholder
// AI.h
#ifndef AI_H
#define AI_H

#include "Game.h"
#include <utility>

class AI {
private:
    Player aiPlayer;
    int minimax(Game game, bool isMax, int alpha, int beta);
public:
    AI(Player aiPlayer);
    std::pair<int, int> findBestMove(Game game);
};

#endif
