// AI.cpp placeholder
// AI.cpp
#include "AI.h"
#include <limits>

AI::AI(Player p) : aiPlayer(p) {}

int AI::minimax(Game game, bool isMax, int alpha, int beta) {
    Player winner = game.getWinner();
    if (winner == aiPlayer) return 10;
    if (winner != Player::NONE) return -10;
    if (game.isDraw()) return 0;

    int bestScore = isMax ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    Player currentPlayer = isMax ? aiPlayer : (aiPlayer == Player::X ? Player::O : Player::X);

    for (auto move : game.getAvailableMoves()) {
        Game temp = game;
        temp.makeMove(move.first, move.second, currentPlayer); // ← use manual player here
        int score = minimax(temp, !isMax, alpha, beta);

        if (isMax) {
            bestScore = std::max(score, bestScore);
            alpha = std::max(alpha, bestScore);
        } else {
            bestScore = std::min(score, bestScore);
            beta = std::min(beta, bestScore);
        }

        if (beta <= alpha) break;
    }

    return bestScore;
}


std::pair<int, int> AI::findBestMove(Game game) {
    int bestVal = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};

    for (auto move : game.getAvailableMoves()) {
        Game temp = game;
        temp.makeMove(move.first, move.second, aiPlayer);  // ← fix: use aiPlayer explicitly
        int moveVal = minimax(temp, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

        if (moveVal > bestVal) {
            bestVal = moveVal;
            bestMove = move;
        }
    }

    return bestMove;
}

