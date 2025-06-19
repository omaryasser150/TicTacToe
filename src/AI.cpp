#include "AI.h"
#include <climits>
#include <algorithm>
#include <random>
#include <chrono>

AI::AI(Player aiPlayer) : aiPlayer(aiPlayer) {}

std::pair<int, int> AI::findBestMove(Game game) {
    // Get all available moves first
    auto availableMoves = game.getAvailableMoves();
    
    // If no moves available, return invalid move
    if (availableMoves.empty()) {
        return {-1, -1};
    }
    
    // If only one move available, take it
    if (availableMoves.size() == 1) {
        return availableMoves[0];
    }
    
    // Check for immediate winning move
    for (const auto& move : availableMoves) {
        Game tempGame = game;
        tempGame.makeMove(move.first, move.second);
        if (tempGame.isWin(aiPlayer)) {
            return move;
        }
    }
    
    // Check for blocking opponent's winning move
    Player opponent = (aiPlayer == Player::X) ? Player::O : Player::X;
    for (const auto& move : availableMoves) {
        Game tempGame = game;
        // Simulate opponent's move
        Player originalPlayer = tempGame.getCurrentPlayer();
        // Force the opponent's turn to check their winning move
        if (originalPlayer == aiPlayer) {
            // Make a dummy move for AI first, then check opponent
            tempGame.makeMove(move.first, move.second);
            // Undo and check if opponent would win there
            tempGame = game;
            // Manually set the move for opponent to test
            Game testGame = game;
            // Switch to opponent's perspective
            while (testGame.getCurrentPlayer() != opponent && !testGame.getAvailableMoves().empty()) {
                auto dummyMoves = testGame.getAvailableMoves();
                if (!dummyMoves.empty()) {
                    testGame.makeMove(dummyMoves[0].first, dummyMoves[0].second);
                }
            }
            if (testGame.getCurrentPlayer() == opponent) {
                testGame.makeMove(move.first, move.second);
                if (testGame.isWin(opponent)) {
                    return move;  // Block this winning move
                }
            }
        } else {
            // It's opponent's turn, check if they can win at this position
            tempGame.makeMove(move.first, move.second);
            if (tempGame.isWin(opponent)) {
                return move;  // Block this winning move
            }
        }
    }
    
    // Use minimax with alpha-beta pruning for remaining moves
    int bestScore = (aiPlayer == Player::X) ? INT_MIN : INT_MAX;
    std::vector<std::pair<int, int>> bestMoves;
    
    for (const auto& move : availableMoves) {
        Game tempGame = game;
        tempGame.makeMove(move.first, move.second);
        
        bool isMaximizing = (tempGame.getCurrentPlayer() != aiPlayer);
        int score = minimax(tempGame, isMaximizing, INT_MIN, INT_MAX, 0);
        
        // Apply strategic bonuses
        score += getPositionValue(move.first, move.second);
        
        if (aiPlayer == Player::X) {
            if (score > bestScore) {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back(move);
            } else if (score == bestScore) {
                bestMoves.push_back(move);
            }
        } else {
            if (score < bestScore) {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back(move);
            } else if (score == bestScore) {
                bestMoves.push_back(move);
            }
        }
    }
    
    // If multiple moves have the same score, apply strategic selection
    if (bestMoves.size() > 1) {
        return selectStrategicMove(game, bestMoves);
    }
    
    return bestMoves.empty() ? availableMoves[0] : bestMoves[0];
}

int AI::minimax(Game game, bool isMaximizing, int alpha, int beta, int depth) {
    // Terminal states
    if (game.isWin(Player::X)) {
        return 10 - depth;  // Prefer quicker wins
    }
    if (game.isWin(Player::O)) {
        return -10 + depth;  // Prefer avoiding quicker losses
    }
    if (game.isDraw()) {
        return 0;
    }
    
    // Depth limit to prevent excessive computation
    if (depth >= 9) {
        return evaluatePosition(game);
    }
    
    auto availableMoves = game.getAvailableMoves();
    
    if (isMaximizing) {
        int maxScore = INT_MIN;
        for (const auto& move : availableMoves) {
            Game tempGame = game;
            tempGame.makeMove(move.first, move.second);
            int score = minimax(tempGame, false, alpha, beta, depth + 1);
            maxScore = std::max(maxScore, score);
            alpha = std::max(alpha, score);
            if (beta <= alpha) {
                break;  // Alpha-beta pruning
            }
        }
        return maxScore;
    } else {
        int minScore = INT_MAX;
        for (const auto& move : availableMoves) {
            Game tempGame = game;
            tempGame.makeMove(move.first, move.second);
            int score = minimax(tempGame, true, alpha, beta, depth + 1);
            minScore = std::min(minScore, score);
            beta = std::min(beta, score);
            if (beta <= alpha) {
                break;  // Alpha-beta pruning
            }
        }
        return minScore;
    }
}

int AI::evaluatePosition(const Game& game) {
    int score = 0;
    
    // Evaluate all possible lines (rows, columns, diagonals)
    // Rows
    for (int i = 0; i < 3; i++) {
        score += evaluateLine(game.at(i, 0), game.at(i, 1), game.at(i, 2));
    }
    
    // Columns
    for (int j = 0; j < 3; j++) {
        score += evaluateLine(game.at(0, j), game.at(1, j), game.at(2, j));
    }
    
    // Diagonals
    score += evaluateLine(game.at(0, 0), game.at(1, 1), game.at(2, 2));
    score += evaluateLine(game.at(0, 2), game.at(1, 1), game.at(2, 0));
    
    return score;
}

int AI::evaluateLine(Player p1, Player p2, Player p3) {
    int score = 0;
    
    // Count X's and O's in the line
    int xCount = 0, oCount = 0;
    if (p1 == Player::X) xCount++;
    else if (p1 == Player::O) oCount++;
    
    if (p2 == Player::X) xCount++;
    else if (p2 == Player::O) oCount++;
    
    if (p3 == Player::X) xCount++;
    else if (p3 == Player::O) oCount++;
    
    // If both X and O are in the line, it's blocked (neutral)
    if (xCount > 0 && oCount > 0) {
        return 0;
    }
    
    // Score based on potential
    if (xCount == 3) return 100;        // X wins
    else if (xCount == 2) return 10;    // X has 2 in a row
    else if (xCount == 1) return 1;     // X has 1 in a row
    
    if (oCount == 3) return -100;       // O wins
    else if (oCount == 2) return -10;   // O has 2 in a row
    else if (oCount == 1) return -1;    // O has 1 in a row
    
    return 0;  // Empty line
}

int AI::getPositionValue(int row, int col) {
    // Strategic position values
    // Center is most valuable, corners are next, edges are least valuable
    if (row == 1 && col == 1) return 4;  // Center
    if ((row == 0 || row == 2) && (col == 0 || col == 2)) return 3;  // Corners
    return 1;  // Edges
}

std::pair<int, int> AI::selectStrategicMove(const Game& game, const std::vector<std::pair<int, int>>& moves) {
    // Priority order for strategic selection:
    // 1. Center (if AI is X and it's the first or second move)
    // 2. Corners when opponent has center
    // 3. Highest position value
    
    // Check if center is available and strategic
    for (const auto& move : moves) {
        if (move.first == 1 && move.second == 1) {
            return move;  // Always prefer center if it's among best moves
        }
    }
    
    // If opponent has center, prefer corners
    if (game.at(1, 1) != Player::NONE && game.at(1, 1) != aiPlayer) {
        for (const auto& move : moves) {
            if ((move.first == 0 || move.first == 2) && (move.second == 0 || move.second == 2)) {
                return move;
            }
        }
    }
    
    // Find move with highest position value
    auto bestMove = moves[0];
    int bestValue = getPositionValue(bestMove.first, bestMove.second);
    
    for (const auto& move : moves) {
        int value = getPositionValue(move.first, move.second);
        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
        }
    }
    
    return bestMove;
}