// test_game.cpp
#include "Game.h"
#include <iostream>

// External declarations from test_main.cpp
extern class TestRunner testRunner;
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }
#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Assertion failed: " #condition " should be false"); \
    }
#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error("Assertion failed: expected " + std::to_string((int)expected) + " but got " + std::to_string((int)actual)); \
    }

void testGameInitialization() {
    Game game;
    
    // Test initial state
    ASSERT_EQ(Player::X, game.getCurrentPlayer());
    ASSERT_EQ(Player::NONE, game.getWinner());
    ASSERT_FALSE(game.isDraw());
    
    // Test all positions are empty
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(Player::NONE, game.at(i, j));
        }
    }
    
    // Test available moves (should be all 9 positions)
    auto moves = game.getAvailableMoves();
    ASSERT_EQ(9, (int)moves.size());
}

void testGameBasicMoves() {
    Game game;
    
    // Test first move (X)
    ASSERT_TRUE(game.makeMove(0, 0));
    ASSERT_EQ(Player::X, game.at(0, 0));
    ASSERT_EQ(Player::O, game.getCurrentPlayer());
    
    // Test second move (O)
    ASSERT_TRUE(game.makeMove(1, 1));
    ASSERT_EQ(Player::O, game.at(1, 1));
    ASSERT_EQ(Player::X, game.getCurrentPlayer());
    
    // Test invalid move (occupied position)
    ASSERT_FALSE(game.makeMove(0, 0));
    ASSERT_EQ(Player::X, game.getCurrentPlayer()); // Should not change
    
    // Test available moves count
    auto moves = game.getAvailableMoves();
    ASSERT_EQ(7, (int)moves.size());
}

void testGameWinConditions() {
    // Test horizontal win
    {
        Game game;
        // X wins top row
        game.makeMove(0, 0); // X
        game.makeMove(1, 0); // O
        game.makeMove(0, 1); // X
        game.makeMove(1, 1); // O
        game.makeMove(0, 2); // X wins
        
        ASSERT_TRUE(game.isWin(Player::X));
        ASSERT_EQ(Player::X, game.getWinner());
        ASSERT_FALSE(game.isDraw());
    }
    
    // Test vertical win
    {
        Game game;
        // O wins left column
        game.makeMove(1, 1); // X
        game.makeMove(0, 0); // O
        game.makeMove(1, 2); // X
        game.makeMove(1, 0); // O
        game.makeMove(2, 2); // X
        game.makeMove(2, 0); // O wins
        
        ASSERT_TRUE(game.isWin(Player::O));
        ASSERT_EQ(Player::O, game.getWinner());
        ASSERT_FALSE(game.isDraw());
    }
    
    // Test diagonal win (top-left to bottom-right)
    {
        Game game;
        // X wins main diagonal
        game.makeMove(0, 0); // X
        game.makeMove(0, 1); // O
        game.makeMove(1, 1); // X
        game.makeMove(0, 2); // O
        game.makeMove(2, 2); // X wins
        
        ASSERT_TRUE(game.isWin(Player::X));
        ASSERT_EQ(Player::X, game.getWinner());
    }
    
    // Test diagonal win (top-right to bottom-left)
    {
        Game game;
        // O wins anti-diagonal
        game.makeMove(0, 0); // X
        game.makeMove(0, 2); // O
        game.makeMove(0, 1); // X
        game.makeMove(1, 1); // O
        game.makeMove(1, 0); // X
        game.makeMove(2, 0); // O wins
        
        ASSERT_TRUE(game.isWin(Player::O));
        ASSERT_EQ(Player::O, game.getWinner());
    }
}

void testGameDraw() {
    Game game;
    
    // Create a draw scenario
    // Final board:
    // X O X
    // O O X
    // O X O
    game.makeMove(0, 0); // X
    game.makeMove(0, 1); // O
    game.makeMove(0, 2); // X
    game.makeMove(1, 0); // O
    game.makeMove(1, 2); // X
    game.makeMove(2, 0); // O
    game.makeMove(2, 1); // X
    game.makeMove(2, 2); // O
    game.makeMove(1, 1); // X
    
    ASSERT_TRUE(game.isDraw());
    ASSERT_EQ(Player::NONE, game.getWinner());
    ASSERT_FALSE(game.isWin(Player::X));
    ASSERT_FALSE(game.isWin(Player::O));
}

void testGameReset() {
    Game game;
    
    // Make some moves
    game.makeMove(0, 0);
    game.makeMove(1, 1);
    game.makeMove(2, 2);
    
    // Reset the game
    game.reset();
    
    // Test reset state
    ASSERT_EQ(Player::X, game.getCurrentPlayer());
    ASSERT_EQ(Player::NONE, game.getWinner());
    ASSERT_FALSE(game.isDraw());
    
    // Test all positions are empty again
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(Player::NONE, game.at(i, j));
        }
    }
    
    // Test available moves
    auto moves = game.getAvailableMoves();
    ASSERT_EQ(9, (int)moves.size());
}

void testGameAvailableMoves() {
    Game game;
    
    // Initially all 9 moves available
    auto moves = game.getAvailableMoves();
    ASSERT_EQ(9, (int)moves.size());
    
    // Make a move
    game.makeMove(1, 1);
    moves = game.getAvailableMoves();
    ASSERT_EQ(8, (int)moves.size());
    
    // Verify center position is not in available moves
    for (const auto& move : moves) {
        ASSERT_FALSE(move.first == 1 && move.second == 1);
    }
    
    // Fill more positions
    game.makeMove(0, 0);
    game.makeMove(2, 2);
    moves = game.getAvailableMoves();
    ASSERT_EQ(6, (int)moves.size());
}

void testGameBoundaryChecking() {
    Game game;
    
    // Test all valid positions
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Game testGame;
            ASSERT_TRUE(testGame.makeMove(i, j));
        }
    }
    
    // Note: The current implementation doesn't explicitly check bounds
    // in makeMove, so we can't test invalid indices without risking crashes
    // In a production system, you'd want bounds checking
}

void testGameComplexScenario() {
    Game game;
    
    // Play a complex game scenario
    // X O X
    // O X O
    // ? ? X  <- X should win with (2,2)
    
    game.makeMove(0, 0); // X
    game.makeMove(0, 1); // O
    game.makeMove(1, 1); // X
    game.makeMove(1, 0); // O
    game.makeMove(0, 2); // X
    game.makeMove(1, 2); // O
    game.makeMove(2, 2); // X wins (diagonal)
    
    ASSERT_EQ(Player::X, game.getWinner());
    ASSERT_TRUE(game.isWin(Player::X));
    ASSERT_FALSE(game.isDraw());
}

void testGamePlayerAlternation() {
    Game game;
    
    // Test that players alternate properly
    ASSERT_EQ(Player::X, game.getCurrentPlayer());
    
    game.makeMove(0, 0);
    ASSERT_EQ(Player::O, game.getCurrentPlayer());
    
    game.makeMove(0, 1);
    ASSERT_EQ(Player::X, game.getCurrentPlayer());
    
    game.makeMove(0, 2);
    ASSERT_EQ(Player::O, game.getCurrentPlayer());
    
    // Failed move shouldn't change current player
    ASSERT_FALSE(game.makeMove(0, 0)); // Already occupied
    ASSERT_EQ(Player::O, game.getCurrentPlayer()); // Should still be O's turn
}

void runGameTests() {
    testRunner.addTest("Game Initialization", testGameInitialization);
    testRunner.addTest("Game Basic Moves", testGameBasicMoves);
    testRunner.addTest("Game Win Conditions", testGameWinConditions);
    testRunner.addTest("Game Draw", testGameDraw);
    testRunner.addTest("Game Reset", testGameReset);
    testRunner.addTest("Game Available Moves", testGameAvailableMoves);
    testRunner.addTest("Game Boundary Checking", testGameBoundaryChecking);
    testRunner.addTest("Game Complex Scenario", testGameComplexScenario);
    testRunner.addTest("Game Player Alternation", testGamePlayerAlternation);
}