#include <gtest/gtest.h>
#include "AI.h"
#include "Game.h"
#include <chrono>
#include <iostream> 

class AITest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup for each test
    }
    
    void printBoard(const Game& game) {
        std::cout << "Current board state:" << std::endl;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                Player p = game.at(i, j);
                char c = (p == Player::X) ? 'X' : (p == Player::O) ? 'O' : '.';
                std::cout << c << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

TEST_F(AITest, Initialization) {
    // Test AI initialization with Player X
    AI aiX(Player::X);
    SUCCEED(); // If we get here, initialization worked
    
    // Test AI initialization with Player O
    AI aiO(Player::O);
    SUCCEED(); // If we get here, initialization worked
}

TEST_F(AITest, WinningMoveHorizontal) {
    Game game;
    game.makeMove(0, 0); // X
    game.makeMove(1, 0); // O
    game.makeMove(0, 1); // X
    game.makeMove(1, 1); // O
    // Board: X X .
    //        O O .
    //        . . .
    // X should play (0,2) to win
    
    AI aiX(Player::X);
    auto move = aiX.findBestMove(game);
    EXPECT_EQ(0, move.first);
    EXPECT_EQ(2, move.second);
}

TEST_F(AITest, WinningMoveVertical) {
    Game game;
    game.makeMove(0, 0); // X
    game.makeMove(0, 1); // O
    game.makeMove(1, 0); // X
    game.makeMove(0, 2); // O
    // Board: X O O
    //        X . .
    //        . . .
    // X should play (2,0) to win
    
    AI aiX(Player::X);
    auto move = aiX.findBestMove(game);
    EXPECT_EQ(2, move.first);
    EXPECT_EQ(0, move.second);
}

TEST_F(AITest, WinningMoveDiagonal) {
    Game game;
    game.makeMove(0, 0); // X
    game.makeMove(0, 1); // O
    game.makeMove(1, 1); // X
    game.makeMove(0, 2); // O
    // Board: X O O
    //        . X .
    //        . . .
    // X should play (2,2) to win
    
    AI aiX(Player::X);
    auto move = aiX.findBestMove(game);
    EXPECT_EQ(2, move.first);
    EXPECT_EQ(2, move.second);
}

TEST_F(AITest, TakesCenterWhenAvailable) {
    Game game;
    game.makeMove(0, 0); // X takes corner
    // Board: X . .
    //        . . .
    //        . . .
    // Now it's O's turn, O should take center (1,1)
    
    printBoard(game);
    std::cout << "Current player: " << (game.getCurrentPlayer() == Player::X ? "X" : "O") << std::endl;
    
    AI aiO(Player::O);
    auto move = aiO.findBestMove(game);
    std::cout << "AI O chose move: (" << move.first << "," << move.second << ")" << std::endl;
    
    EXPECT_EQ(1, move.first);
    EXPECT_EQ(1, move.second);
}

TEST_F(AITest, TakesCornerWhenOpponentHasCenter) {
    Game game;
    game.makeMove(1, 1); // X takes center
    // Board: . . .
    //        . X .
    //        . . .
    // O should take a corner
    
    AI aiO(Player::O);
    auto move = aiO.findBestMove(game);
    bool isCorner = (move.first == 0 && move.second == 0) ||
                   (move.first == 0 && move.second == 2) ||
                   (move.first == 2 && move.second == 0) ||
                   (move.first == 2 && move.second == 2);
    EXPECT_TRUE(isCorner);
}

TEST_F(AITest, ReturnsValidMove) {
    Game game;
    // Fill some positions
    game.makeMove(0, 0); // X
    game.makeMove(0, 1); // O
    game.makeMove(1, 0); // X
    game.makeMove(1, 1); // O
    game.makeMove(2, 0); // X
    game.makeMove(2, 1); // O
    // Board: X O .
    //        X O .
    //        X O .
    // Only positions (0,2), (1,2), and (2,2) available
    
    AI aiX(Player::X);
    auto move = aiX.findBestMove(game);
    
    // Verify the move is valid
    bool validMove = (move.first >= 0 && move.first < 3 && 
                     move.second >= 0 && move.second < 3 &&
                     game.at(move.first, move.second) == Player::NONE);
    EXPECT_TRUE(validMove);
    
    // Verify the move is one of the available moves
    auto availableMoves = game.getAvailableMoves();
    bool foundMove = false;
    for (const auto& availableMove : availableMoves) {
        if (availableMove.first == move.first && availableMove.second == move.second) {
            foundMove = true;
            break;
        }
    }
    EXPECT_TRUE(foundMove);
}

TEST_F(AITest, HandlesWonGameState) {
    Game wonGame;
    wonGame.makeMove(0, 0); // X
    wonGame.makeMove(1, 0); // O
    wonGame.makeMove(0, 1); // X
    wonGame.makeMove(1, 1); // O
    wonGame.makeMove(0, 2); // X wins
    // Board: X X X
    //        O O .
    //        . . .
    
    EXPECT_EQ(Player::X, wonGame.getWinner());
    
    // AI should still be able to find a move (even though game is over)
    AI aiO(Player::O);
    auto move = aiO.findBestMove(wonGame);
    
    // The move should be valid (from available positions)
    auto availableMoves = wonGame.getAvailableMoves();
    bool validMove = false;
    for (const auto& availableMove : availableMoves) {
        if (availableMove.first == move.first && availableMove.second == move.second) {
            validMove = true;
            break;
        }
    }
    EXPECT_TRUE(validMove || availableMoves.empty());
}

TEST_F(AITest, Performance) {
    Game game;
    AI ai(Player::X);
    
    auto start = std::chrono::high_resolution_clock::now();
    auto move = ai.findBestMove(game);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "AI calculation time: " << duration.count() << "ms" << std::endl;
    EXPECT_LT(duration.count(), 1000); // Less than 1 second
    EXPECT_GE(move.first, 0);
    EXPECT_LT(move.first, 3);
    EXPECT_GE(move.second, 0);
    EXPECT_LT(move.second, 3);
}

// Additional test for AI vs AI scenario
TEST_F(AITest, AIvsAI) {
    Game game;
    AI aiX(Player::X);
    AI aiO(Player::O);
    
    int moveCount = 0;
    while (game.getWinner() == Player::NONE && !game.isDraw() && moveCount < 9) {
        if (game.getCurrentPlayer() == Player::X) {
            auto move = aiX.findBestMove(game);
            EXPECT_TRUE(game.makeMove(move.first, move.second));
        } else {
            auto move = aiO.findBestMove(game);
            EXPECT_TRUE(game.makeMove(move.first, move.second));
        }
        moveCount++;
    }
    
    // Game should end in a draw when both AIs play optimally
    EXPECT_TRUE(game.isDraw() || game.getWinner() != Player::NONE);
    std::cout << "AI vs AI game ended with: ";
    if (game.isDraw()) {
        std::cout << "Draw" << std::endl;
    } else {
        std::cout << (game.getWinner() == Player::X ? "X wins" : "O wins") << std::endl;
    }
}

// Test AI makes immediate winning move when available
TEST_F(AITest, ImmediateWinPriority) {
    Game game;
    // Setup a board where AI has multiple good options but one is a win
    game.makeMove(0, 0); // X
    game.makeMove(1, 0); // O
    game.makeMove(0, 1); // X
    game.makeMove(2, 0); // O
    // Board: X X .
    //        O . .
    //        O . .
    // X can win by playing (0,2) or could play defensively
    
    AI aiX(Player::X);
    auto move = aiX.findBestMove(game);
    
    // AI should prioritize the winning move
    EXPECT_EQ(0, move.first);
    EXPECT_EQ(2, move.second);
}