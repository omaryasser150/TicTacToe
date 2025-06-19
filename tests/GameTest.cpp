// test_game.cpp placeholder
#include <gtest/gtest.h>
#include "../src/Game.h"

TEST(GameTest, WinDetection) {
    Game g;
    g.makeMove(0, 0); // X
    g.makeMove(1, 0); // O
    g.makeMove(0, 1); // X
    g.makeMove(1, 1); // O
    g.makeMove(0, 2); // X wins
    EXPECT_EQ(g.getWinner(), Player::X);
}
