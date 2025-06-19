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
    
    // Determine which player's turn it is
    Player currentPlayer = game.getCurrentPlayer();
    bool isAITurn = (currentPlayer == aiPlayer);
    
    if (isAITurn) {
        // AI's turn - maximize
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
        // Opponent's turn - minimize
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
    
    // First, check for immediate winning moves
    for (const auto& move : availableMoves) {
        Game tempGame = game;
        tempGame.makeMove(move.first, move.second);
        if (tempGame.getWinner() == aiPlayer) {
            return move; // Take the winning move immediately
        }
    }
    
    // Second, check for blocking moves (prevent opponent from winning)
    Player opponent = (aiPlayer == Player::X) ? Player::O : Player::X;
    
    // To check blocking moves, we need to see if the opponent would win
    // if they could make a move at each available position
    for (const auto& move : availableMoves) {
        // Create a test board with the opponent's move
        Game testGame = game;
        
        // Temporarily place opponent's piece to test if it would create a win
        // We need to create a custom test since makeMove uses currentPlayer
        if (wouldOpponentWin(game, move.first, move.second, opponent)) {
            return move; // Block this winning move
        }
    }
    
    // If no immediate win/block, use minimax
    std::pair<int, int> bestMove = availableMoves[0];
    int bestValue = INT_MIN;
    
    for (const auto& move : availableMoves) {
        Game tempGame = game;
        tempGame.makeMove(move.first, move.second);
        
        // After AI makes the move, it's opponent's turn
        int moveValue = minimax(tempGame, false, INT_MIN, INT_MAX, 0);
        
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    
    return bestMove;
}

// Helper function to check if opponent would win with a specific move
bool AI::wouldOpponentWin(const Game& game, int row, int col, Player opponent) {
    // Create a temporary board with the opponent's move
    std::vector<std::vector<Player>> testBoard(3, std::vector<Player>(3, Player::NONE));
    
    // Copy current board
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            testBoard[i][j] = game.at(i, j);
        }
    }
    
    // Add opponent's move
    testBoard[row][col] = opponent;
    
    // Check for wins
    // Check horizontal
    for (int i = 0; i < 3; i++) {
        if (testBoard[i][0] == opponent && testBoard[i][1] == opponent && testBoard[i][2] == opponent) {
            return true;
        }
    }
    
    // Check vertical
    for (int j = 0; j < 3; j++) {
        if (testBoard[0][j] == opponent && testBoard[1][j] == opponent && testBoard[2][j] == opponent) {
            return true;
        }
    }
    
    // Check diagonals
    if ((testBoard[0][0] == opponent && testBoard[1][1] == opponent && testBoard[2][2] == opponent) ||
        (testBoard[0][2] == opponent && testBoard[1][1] == opponent && testBoard[2][0] == opponent)) {
        return true;
    }
    
    return false;
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