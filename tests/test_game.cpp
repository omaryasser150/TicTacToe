#include <gtest/gtest.h>
#include "Game.h"

class GameTest : public ::testing::Test {
protected:
    Game game; // A fresh game instance for each test
    
    void SetUp() override {
        game.reset(); // Ensure game is reset before each test
    }
};

TEST_F(GameTest, Initialization) {
    // Test initial state
    EXPECT_EQ(Player::X, game.getCurrentPlayer());
    EXPECT_EQ(Player::NONE, game.getWinner());
    EXPECT_FALSE(game.isDraw());
    
    // Test all positions are empty
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(Player::NONE, game.at(i, j));
        }
    }
    
    // Test available moves (should be all 9 positions)
    auto moves = game.getAvailableMoves();
    EXPECT_EQ(9, moves.size());
}

TEST_F(GameTest, BasicMoves) {
    // Test first move (X)
    EXPECT_TRUE(game.makeMove(0, 0));
    EXPECT_EQ(Player::X, game.at(0, 0));
    EXPECT_EQ(Player::O, game.getCurrentPlayer()); // O’s turn
    
    // Test second move (O)
    EXPECT_TRUE(game.makeMove(1, 1));
    EXPECT_EQ(Player::O, game.at(1, 1));
    EXPECT_EQ(Player::X, game.getCurrentPlayer()); // X’s turn
    
    // Test invalid move (occupied position)
    EXPECT_FALSE(game.makeMove(0, 0));
    EXPECT_EQ(Player::X, game.getCurrentPlayer()); // Should not change
    
    // Test available moves count
    auto moves = game.getAvailableMoves();
    EXPECT_EQ(7, moves.size());
}

TEST_F(GameTest, HorizontalWin) {
    // X wins top row
    game.makeMove(0, 0); // X
    game.makeMove(1, 0); // O
    game.makeMove(0, 1); // X
    game.makeMove(1, 1); // O
    game.makeMove(0, 2); // X wins
    
    EXPECT_TRUE(game.isWin(Player::X));
    EXPECT_EQ(Player::X, game.getWinner());
    EXPECT_FALSE(game.isDraw());
}

TEST_F(GameTest, VerticalWin) {
    // O wins left column
    game.makeMove(1, 1); // X
    game.makeMove(0, 0); // O
    game.makeMove(1, 2); // X
    game.makeMove(1, 0); // O
    game.makeMove(2, 2); // X
    game.makeMove(2, 0); // O wins
    
    EXPECT_TRUE(game.isWin(Player::O));
    EXPECT_EQ(Player::O, game.getWinner());
    EXPECT_FALSE(game.isDraw());
}

TEST_F(GameTest, DiagonalWinMain) {
    // X wins main diagonal (top-left to bottom-right)
    game.makeMove(0, 0); // X
    game.makeMove(0, 1); // O
    game.makeMove(1, 1); // X
    game.makeMove(0, 2); // O
    game.makeMove(2, 2); // X wins
    
    EXPECT_TRUE(game.isWin(Player::X));
    EXPECT_EQ(Player::X, game.getWinner());
}

TEST_F(GameTest, DiagonalWinAnti) {
    // O wins anti-diagonal (top-right to bottom-left)
    game.makeMove(0, 0); // X
    game.makeMove(0, 2); // O
    game.makeMove(0, 1); // X
    game.makeMove(1, 1); // O
    game.makeMove(1, 0); // X
    game.makeMove(2, 0); // O wins
    
    EXPECT_TRUE(game.isWin(Player::O));
    EXPECT_EQ(Player::O, game.getWinner());
}

TEST_F(GameTest, Draw) {
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
    
    EXPECT_TRUE(game.isDraw());
    EXPECT_EQ(Player::NONE, game.getWinner());
    EXPECT_FALSE(game.isWin(Player::X));
    EXPECT_FALSE(game.isWin(Player::O));
}


TEST_F(GameTest, AvailableMoves) {
    // Initially all 9 moves available
    auto moves = game.getAvailableMoves();
    EXPECT_EQ(9, moves.size());
    
    // Make a move
    game.makeMove(1, 1);
    moves = game.getAvailableMoves();
    EXPECT_EQ(8, moves.size());
    
    // Verify center position is not in available moves
    for (const auto& move : moves) {
        EXPECT_FALSE(move.first == 1 && move.second == 1);
    }
    
    // Fill more positions
    game.makeMove(0, 0);
    game.makeMove(2, 2);
    moves = game.getAvailableMoves();
    EXPECT_EQ(6, moves.size());
}


TEST_F(GameTest, PlayerAlternation) {
    // Test that players alternate properly
    EXPECT_EQ(Player::X, game.getCurrentPlayer());
    
    game.makeMove(0, 0);
    EXPECT_EQ(Player::O, game.getCurrentPlayer());
    
    game.makeMove(0, 1);
    EXPECT_EQ(Player::X, game.getCurrentPlayer());
    
    game.makeMove(0, 2);
    EXPECT_EQ(Player::O, game.getCurrentPlayer());
    
    // Failed move shouldn't change current player
    EXPECT_FALSE(game.makeMove(0, 0)); // Already occupied
    EXPECT_EQ(Player::O, game.getCurrentPlayer()); // Should still be O's turn
}