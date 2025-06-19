#include "AI.h"
#include <climits>
#include <algorithm>

AI::AI(Player aiPlayer) : aiPlayer(aiPlayer) {}

int AI::minimax(Game game, bool isMaximizing, int alpha, int beta, int depth) {
    // Terminal states
    Player winner = game.getWinner();
    if (winner == aiPlayer) {
        return 10 - depth; // Prefer quicker wins
    } else if (winner != Player::NONE && winner != aiPlayer) {
        return depth - 10; // Prefer later losses
    } else if (game.isDraw()) {
        return 0; // Draw
    }
    
    // Depth limit to prevent infinite recursion
    if (depth > 9) {
        return 0;
    }
    
    std::vector<std::pair<int, int>> availableMoves = game.getAvailableMoves();
    
    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (const auto& move : availableMoves) {
            Game tempGame = game;
            tempGame.makeMove(move.first, move.second);
            int eval = minimax(tempGame, false, alpha, beta, depth + 1);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const auto& move : availableMoves) {
            Game tempGame = game;
            tempGame.makeMove(move.first, move.second);
            int eval = minimax(tempGame, true, alpha, beta, depth + 1);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        return minEval;
    }
}

std::pair<int, int> AI::findBestMove(Game game) {
    std::vector<std::pair<int, int>> availableMoves = game.getAvailableMoves();
    
    // If no moves available, return invalid move
    if (availableMoves.empty()) {
        return std::make_pair(-1, -1);
    }
    
    // If only one move available, take it
    if (availableMoves.size() == 1) {
        return availableMoves[0];
    }
    
    std::pair<int, int> bestMove = availableMoves[0];
    int bestValue = INT_MIN;
    
    for (const auto& move : availableMoves) {
        Game tempGame = game;
        tempGame.makeMove(move.first, move.second);
        
        // After making the move, it's the opponent's turn, so we minimize
        int moveValue = minimax(tempGame, false, INT_MIN, INT_MAX, 0);
        
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    
    return bestMove;
}

// Strategic evaluation for early game optimization
int AI::evaluatePosition(const Game& game) {
    int score = 0;
    
    // Center position is valuable
    if (game.at(1, 1) == aiPlayer) {
        score += 3;
    } else if (game.at(1, 1) != Player::NONE) {
        score -= 3;
    }
    
    // Corners are valuable
    std::vector<std::pair<int, int>> corners = {{0,0}, {0,2}, {2,0}, {2,2}};
    for (const auto& corner : corners) {
        if (game.at(corner.first, corner.second) == aiPlayer) {
            score += 2;
        } else if (game.at(corner.first, corner.second) != Player::NONE) {
            score -= 2;
        }
    }
    
    // Check for potential wins/blocks
    score += evaluateLines(game);
    
    return score;
}

int AI::evaluateLines(const Game& game) {
    int score = 0;
    
    // Check all possible lines (rows, columns, diagonals)
    std::vector<std::vector<std::pair<int, int>>> lines = {
        // Rows
        {{0,0}, {0,1}, {0,2}},
        {{1,0}, {1,1}, {1,2}},
        {{2,0}, {2,1}, {2,2}},
        // Columns
        {{0,0}, {1,0}, {2,0}},
        {{0,1}, {1,1}, {2,1}},
        {{0,2}, {1,2}, {2,2}},
        // Diagonals
        {{0,0}, {1,1}, {2,2}},
        {{0,2}, {1,1}, {2,0}}
    };
    
    for (const auto& line : lines) {
        int aiCount = 0;
        int opponentCount = 0;
        int emptyCount = 0;
        
        for (const auto& pos : line) {
            Player cell = game.at(pos.first, pos.second);
            if (cell == aiPlayer) {
                aiCount++;
            } else if (cell != Player::NONE) {
                opponentCount++;
            } else {
                emptyCount++;
            }
        }
        
        // Scoring based on line evaluation
        if (aiCount == 3) {
            score += 100; // Win
        } else if (aiCount == 2 && emptyCount == 1) {
            score += 10; // Two in a row
        } else if (aiCount == 1 && emptyCount == 2) {
            score += 1; // One in a row
        }
        
        if (opponentCount == 3) {
            score -= 100; // Opponent win
        } else if (opponentCount == 2 && emptyCount == 1) {
            score -= 10; // Block opponent
        } else if (opponentCount == 1 && emptyCount == 2) {
            score -= 1; // Opponent has one
        }
    }
    
    return score;
}