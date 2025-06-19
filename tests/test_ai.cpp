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

TEST_F(AITest, BlockingMoveHorizontal) {
    Game game;
    game.makeMove(0, 0); // X
    game.makeMove(1, 0); // O
    game.makeMove(0, 1); // X
    game.makeMove(1, 1); // O
    game.makeMove(2, 0); // X
    // Board: X X .
    //        O O .
    //        X . .
    // O should play (0,2) to block X's win
    
    AI aiO(Player::O);
    auto move = aiO.findBestMove(game);
    EXPECT_EQ(0, move.first);
    EXPECT_EQ(2, move.second);
}

TEST_F(AITest, BlockingMoveDiagonal) {
    Game game;
    game.makeMove(0, 0); // X
    game.makeMove(0, 1); // O
    game.makeMove(1, 1); // X
    game.makeMove(0, 2); // O
    game.makeMove(1, 0); // X
    // Board: X O O
    //        X X .
    //        . . .
    // O should play (2,2) to block X's diagonal win
    
    AI aiO(Player::O);
    auto move = aiO.findBestMove(game);
    EXPECT_EQ(2, move.first);
    EXPECT_EQ(2, move.second);
}

TEST_F(AITest, TakesCenterWhenAvailable) {
    Game game;
    game.makeMove(0, 0); // X
    game.makeMove(2, 2); // O (AI's turn would be X now)
    // Board: X . .
    //        . . .
    //        . . O
    // X should prefer center (1,1) as it's optimal
    
    AI aiX(Player::X);
    auto move = aiX.findBestMove(game);
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

TEST_F(AITest, DrawScenario) {
    Game game;
    game.makeMove(1, 1); // X takes center
    game.makeMove(0, 0); // O takes corner
    game.makeMove(2, 2); // X takes opposite corner
    game.makeMove(0, 2); // O takes another corner
    game.makeMove(0, 1); // X blocks
    game.makeMove(2, 0); // O takes last corner
    game.makeMove(1, 0); // X blocks
    game.makeMove(1, 2); // O blocks
    // Board: O X O
    //        X X O
    //        O . X
    // Only (2,1) left, should be a draw
    
    AI aiX(Player::X);
    auto move = aiX.findBestMove(game);
    EXPECT_EQ(2, move.first);
    EXPECT_EQ(1, move.second);
    
    // Complete the game and verify it's a draw
    game.makeMove(move.first, move.second);
    EXPECT_TRUE(game.isDraw());
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
    // Only positions (0,2) and (1,2) and (2,2) available
    
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
    EXPECT_LT(duration.count(), 5000); // Less than 5 seconds
    EXPECT_GE(move.first, 0);
    EXPECT_LT(move.first, 3);
    EXPECT_GE(move.second, 0);
    EXPECT_LT(move.second, 3);
}