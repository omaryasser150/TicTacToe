#include <gtest/gtest.h>
#include "Game.h"
#include "AI.h"
#include "Auth.h"
#include "History.h"
#include <filesystem>
#include <thread>
#include <chrono>

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up any existing test files
        std::filesystem::remove("test_integration_users.db");
        std::filesystem::remove("history_testuser.txt");
        std::filesystem::remove("history_player1.txt");
        std::filesystem::remove("history_player2.txt");
    }
    
    void TearDown() override {
        // Clean up test files
        std::filesystem::remove("test_integration_users.db");
        std::filesystem::remove("history_testuser.txt");
        std::filesystem::remove("history_player1.txt");
        std::filesystem::remove("history_player2.txt");
    }
};

// Test 1: Complete User Workflow - Login -> Play Game -> Save History
TEST_F(IntegrationTest, CompleteUserGameWorkflow) {
    // Step 1: User Registration and Login
    Auth auth("test_integration_users.db");
    ASSERT_TRUE(auth.registerUser("testuser", "password123"));
    ASSERT_TRUE(auth.loginUser("testuser", "password123"));
    
    // Step 2: Initialize Game and History
    Game game;
    History history("testuser");
    AI ai(Player::O); // AI plays as O
    
    // Step 3: Play a complete game (User vs AI)
    int moveCount = 0;
    std::string gameTimestamp = "2024-06-20 15:30:00";
    
    while (game.getWinner() == Player::NONE && !game.isDraw() && moveCount < 9) {
        if (game.getCurrentPlayer() == Player::X) {
            // Simulate user move (X)
            auto availableMoves = game.getAvailableMoves();
            ASSERT_FALSE(availableMoves.empty());
            auto userMove = availableMoves[0]; // Take first available move
            ASSERT_TRUE(game.makeMove(userMove.first, userMove.second));
        } else {
            // AI move (O)
            auto aiMove = ai.findBestMove(game);
            ASSERT_TRUE(game.makeMove(aiMove.first, aiMove.second));
        }
        moveCount++;
    }
    
    // Step 4: Determine and save game result
    std::string result;
    if (game.getWinner() == Player::X) {
        result = "Win";
    } else if (game.getWinner() == Player::O) {
        result = "Loss vs AI";
    } else {
        result = "Draw vs AI";
    }
    
    GameResult gameResult = {gameTimestamp, result};
    history.saveResult(gameResult);
    
    // Step 5: Verify the complete workflow
    // Check game completed properly
    EXPECT_TRUE(game.getWinner() != Player::NONE || game.isDraw());
    
    // Check history was saved
    auto loadedHistory = history.loadHistory();
    EXPECT_EQ(1, loadedHistory.size());
    EXPECT_EQ(gameTimestamp, loadedHistory[0].date);
    EXPECT_EQ(result, loadedHistory[0].result);
    
    // Verify user is still authenticated (session persistence)
    EXPECT_TRUE(auth.loginUser("testuser", "password123"));
}

// Test 2: Multi-User Game Session Integration
TEST_F(IntegrationTest, MultiUserGameSession) {
    Auth auth("test_integration_users.db");
    
    // Register two players
    ASSERT_TRUE(auth.registerUser("player1", "pass1"));
    ASSERT_TRUE(auth.registerUser("player2", "pass2"));
    
    // Both players login
    ASSERT_TRUE(auth.loginUser("player1", "pass1"));
    ASSERT_TRUE(auth.loginUser("player2", "pass2"));
    
    // Initialize separate histories
    History history1("player1");
    History history2("player2");
    
    // Play multiple games between players
    for (int gameNum = 0; gameNum < 3; gameNum++) {
        Game game;
        std::string timestamp = "2024-06-20 " + std::to_string(10 + gameNum) + ":00:00";
        
        // Simulate player vs player game
        std::vector<std::pair<int, int>> moves = {
            {0, 0}, {1, 1}, {0, 1}, {1, 0}, {0, 2} // X wins
        };
        
        for (const auto& move : moves) {
            if (game.getWinner() == Player::NONE && !game.isDraw()) {
                game.makeMove(move.first, move.second);
            }
        }
        
        // Save results for both players
        if (game.getWinner() == Player::X) {
            history1.saveResult({timestamp, "Win vs Player2"});
            history2.saveResult({timestamp, "Loss vs Player1"});
        } else if (game.getWinner() == Player::O) {
            history1.saveResult({timestamp, "Loss vs Player2"});
            history2.saveResult({timestamp, "Win vs Player1"});
        } else {
            history1.saveResult({timestamp, "Draw vs Player2"});
            history2.saveResult({timestamp, "Draw vs Player1"});
        }
    }
    
    // Verify both players have complete histories
    auto history1Data = history1.loadHistory();
    auto history2Data = history2.loadHistory();
    
    EXPECT_EQ(3, history1Data.size());
    EXPECT_EQ(3, history2Data.size());
    
    // Verify histories are separate and correct
    for (int i = 0; i < 3; i++) {
        EXPECT_NE(history1Data[i].result, history2Data[i].result); 
        // Results should be opposite (win/loss)
    }
}

// Test 3: Game-AI-History Integration with Performance Tracking
TEST_F(IntegrationTest, GameAIHistoryWithPerformance) {
    Auth auth("test_integration_users.db");
    ASSERT_TRUE(auth.registerUser("perftest", "password"));
    ASSERT_TRUE(auth.loginUser("perftest", "password"));
    
    History history("perftest");
    
    // Play multiple AI games and track performance
    const int numGames = 10;
    std::vector<long> aiResponseTimes;
    
    for (int gameNum = 0; gameNum < numGames; gameNum++) {
        Game game;
        AI ai(Player::O);
        
        auto gameStart = std::chrono::high_resolution_clock::now();
        
        while (game.getWinner() == Player::NONE && !game.isDraw()) {
            if (game.getCurrentPlayer() == Player::X) {
                // Quick user move (first available)
                auto moves = game.getAvailableMoves();
                game.makeMove(moves[0].first, moves[0].second);
            } else {
                // Time AI move
                auto aiStart = std::chrono::high_resolution_clock::now();
                auto aiMove = ai.findBestMove(game);
                auto aiEnd = std::chrono::high_resolution_clock::now();
                
                auto aiTime = std::chrono::duration_cast<std::chrono::milliseconds>(aiEnd - aiStart);
                aiResponseTimes.push_back(aiTime.count());
                
                game.makeMove(aiMove.first, aiMove.second);
            }
        }
        
        auto gameEnd = std::chrono::high_resolution_clock::now();
        auto gameDuration = std::chrono::duration_cast<std::chrono::seconds>(gameEnd - gameStart);
        
        // Save game result with performance data
        std::string result = "Game " + std::to_string(gameNum + 1) + 
                           " (" + std::to_string(gameDuration.count()) + "s)";
        if (game.getWinner() == Player::X) result += " - Win";
        else if (game.getWinner() == Player::O) result += " - Loss";
        else result += " - Draw";
        
        history.saveResult({"2024-06-20 12:00:00", result});
    }
    
    // Verify performance requirements
    long totalAITime = 0;
    long maxAITime = 0;
    for (long time : aiResponseTimes) {
        totalAITime += time;
        maxAITime = std::max(maxAITime, time);
    }
    
    long avgAITime = totalAITime / aiResponseTimes.size();
    
    // Performance assertions
    EXPECT_LT(avgAITime, 500); // Average AI response < 500ms
    EXPECT_LT(maxAITime, 1000); // Max AI response < 1000ms
    
    // Verify all games were recorded
    auto gameHistory = history.loadHistory();
    EXPECT_EQ(numGames, gameHistory.size());
    
    std::cout << "AI Performance Stats:" << std::endl;
    std::cout << "Average AI response time: " << avgAITime << "ms" << std::endl;
    std::cout << "Maximum AI response time: " << maxAITime << "ms" << std::endl;
}

// Test 4: Error Handling Integration
TEST_F(IntegrationTest, ErrorHandlingIntegration) {
    // Test system behavior when components fail
    
    // 1. Authentication failure should not crash game
    Auth auth("test_integration_users.db");
    ASSERT_TRUE(auth.registerUser("errortest", "password"));
    
    // Simulate auth failure during game
    Game game;
    History history("errortest");
    
    // Game should continue even if auth has issues
    EXPECT_TRUE(game.makeMove(0, 0));
    EXPECT_TRUE(game.makeMove(1, 1));
    
    // 2. History save failure should not crash game
    // Create invalid history (readonly directory simulation)
    History invalidHistory("/root/readonly/history"); // This should fail gracefully
    
    // Game should still work
    EXPECT_TRUE(game.makeMove(0, 1));
    EXPECT_TRUE(game.makeMove(1, 0));
    
    // 3. AI failure should not crash game
    AI ai(Player::O);
    
    // Even with edge cases, AI should return valid move or handle gracefully
    Game edgeGame;
    // Fill most of board
    edgeGame.makeMove(0, 0); edgeGame.makeMove(0, 1);
    edgeGame.makeMove(0, 2); edgeGame.makeMove(1, 0);
    edgeGame.makeMove(1, 2); edgeGame.makeMove(2, 0);
    edgeGame.makeMove(2, 1); // Only (1,1) and (2,2) left
    
    auto aiMove = ai.findBestMove(edgeGame);
    EXPECT_TRUE((aiMove.first == 1 && aiMove.second == 1) || 
                (aiMove.first == 2 && aiMove.second == 2));
}

// Test 5: Data Consistency Integration
TEST_F(IntegrationTest, DataConsistencyIntegration) {
    Auth auth("test_integration_users.db");
    ASSERT_TRUE(auth.registerUser("consistency", "password"));
    
    History history("consistency");
    
    // Play games and verify data consistency across components
    for (int i = 0; i < 5; i++) {
        Game game;
        AI ai(Player::O);
        
        // Quick game simulation
        while (game.getWinner() == Player::NONE && !game.isDraw()) {
            if (game.getCurrentPlayer() == Player::X) {
                auto moves = game.getAvailableMoves();
                game.makeMove(moves[0].first, moves[0].second);
            } else {
                auto aiMove = ai.findBestMove(game);
                game.makeMove(aiMove.first, aiMove.second);
            }
        }
        
        // Save result
        std::string result = (game.getWinner() == Player::X) ? "Win" : 
                           (game.getWinner() == Player::O) ? "Loss" : "Draw";
        history.saveResult({"2024-06-20", result});
    }
    
    // Verify data consistency
    auto historyData = history.loadHistory();
    EXPECT_EQ(5, historyData.size());
    
    // All results should be valid game outcomes
    for (const auto& record : historyData) {
        EXPECT_TRUE(record.result == "Win" || 
                   record.result == "Loss" || 
                   record.result == "Draw");
    }
    
    // User should still exist and be authenticatable
    EXPECT_TRUE(auth.loginUser("consistency", "password"));
}