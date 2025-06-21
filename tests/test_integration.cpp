#include <gtest/gtest.h>
#include "Auth.h"
#include "Game.h"
#include "AI.h"
#include "History.h"
#include <filesystem>
#include <chrono>               // Used in performance test or creating timestamps
#include <iostream>
#include <sstream>              //sed to format strings (especially timestamps) in a flexible way

class IntegrationTest : public ::testing::Test {
protected:
    std::vector<std::string> testFiles; // Track test files for cleanup

    void SetUp() override {
        // Setup for each test
        cleanupTestFiles();
    }
    
    void TearDown() override {
        // Clean up test files
        cleanupTestFiles();
    }

    void addTestFile(const std::string& filename) {
        testFiles.push_back(filename);
    }

    void cleanupTestFiles() {
        // Clean up specific test files and any that might have been created
        std::vector<std::string> filesToRemove = {
            "integration_test_users.db",
            "test_integration_users.db",
            "game_session_users.db",
            "tournament_users.db",
            "history_alice.txt",
            "history_bob.txt",
            "history_testplayer1.txt",
            "history_testplayer2.txt",
            "history_player1.txt",
            "history_player2.txt",
            "history_integration_user.txt"
        };
        
        // Add tracked files
        filesToRemove.insert(filesToRemove.end(), testFiles.begin(), testFiles.end());
        
        for (const auto& filename : filesToRemove) {
            if (std::filesystem::exists(filename)) {
                std::filesystem::remove(filename);
            }
        }
        testFiles.clear();
    }

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    //Prints the current Tic Tac Toe board visually to the console (for debugging or analysis)
    void printGameBoard(const Game& game) {
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

TEST_F(IntegrationTest, CompleteUserSession) {
    // Test a complete user session: register, login, play games, save history
    std::string dbFile = "integration_test_users.db";
    std::string username = "integration_user";
    std::string password = "testpass123";
    
    addTestFile(dbFile);
    addTestFile("history_" + username + ".txt");
    
    // 1. User Registration
    Auth auth(dbFile);
    auth.clearAllUsers();
    ASSERT_TRUE(auth.registerUser(username, password));
    
    // 2. User Login
    ASSERT_TRUE(auth.loginUser(username, password));
    
    // 3. Initialize game components
    Game game;
    AI ai(Player::O);
    History history(username);
    
    // 4. Play a complete game (Human vs AI)
    std::cout << "=== Playing Human vs AI Game ===" << std::endl;
    
    // Human (X) makes first move
    ASSERT_TRUE(game.makeMove(1, 1)); // Take center
    printGameBoard(game);
    
    int moveCount = 1;
    while (game.getWinner() == Player::NONE && !game.isDraw() && moveCount < 9) {
        if (game.getCurrentPlayer() == Player::O) {
            // AI's turn
            auto aiMove = ai.findBestMove(game);
            std::cout << "AI plays: (" << aiMove.first << "," << aiMove.second << ")" << std::endl;
            ASSERT_TRUE(game.makeMove(aiMove.first, aiMove.second));
            printGameBoard(game);
        } else {
            // Simulate human moves for testing
            auto availableMoves = game.getAvailableMoves();
            if (!availableMoves.empty()) {
                auto humanMove = availableMoves[0]; // Take first available
                std::cout << "Human plays: (" << humanMove.first << "," << humanMove.second << ")" << std::endl;
                ASSERT_TRUE(game.makeMove(humanMove.first, humanMove.second));
                printGameBoard(game);
            }
        }
        moveCount++;
    }
    
    // 5. Determine and save game result
    std::string result;
    if (game.getWinner() == Player::X) {
        result = "Win";
        std::cout << "Human wins!" << std::endl;
    } else if (game.getWinner() == Player::O) {
        result = "Loss";
        std::cout << "AI wins!" << std::endl;
    } else {
        result = "Draw";
        std::cout << "Game is a draw!" << std::endl;
    }
    
    // 6. Save result to history
    GameResult gameResult = {getCurrentTimestamp(), result};
    history.saveResult(gameResult);
    
    // 7. Verify history was saved
    auto loadedHistory = history.loadHistory();
    EXPECT_EQ(1, loadedHistory.size());
    if (!loadedHistory.empty()) {
        EXPECT_EQ(result, loadedHistory[0].result);
    }
    
    std::cout << "=== Session Complete ===" << std::endl;
}

TEST_F(IntegrationTest, MultiPlayerGameSession) {
    // Test multiple players registering and playing games
    std::string dbFile = "game_session_users.db";
    addTestFile(dbFile);
    addTestFile("history_alice.txt");
    addTestFile("history_bob.txt");
    
    Auth auth(dbFile);
    auth.clearAllUsers();
    
    // Register two players
    ASSERT_TRUE(auth.registerUser("alice", "password1"));
    ASSERT_TRUE(auth.registerUser("bob", "password2"));
    
    // Both players login
    ASSERT_TRUE(auth.loginUser("alice", "password1"));
    ASSERT_TRUE(auth.loginUser("bob", "password2"));
    
    // Create history objects for both players
    History aliceHistory("alice");
    History bobHistory("bob");
    
    // Play multiple games between Alice (X) and Bob (O)
    for (int gameNum = 1; gameNum <= 3; gameNum++) {
        std::cout << "=== Game " << gameNum << " ===" << std::endl;
        
        Game game;
        Player alicePlayer = (gameNum % 2 == 1) ? Player::X : Player::O;
        Player bobPlayer = (gameNum % 2 == 1) ? Player::O : Player::X;
        
        std::cout << "Alice is " << (alicePlayer == Player::X ? "X" : "O") << std::endl;
        std::cout << "Bob is " << (bobPlayer == Player::X ? "X" : "O") << std::endl;
        
        // Simulate alternating moves
        int moveCount = 0;
        while (game.getWinner() == Player::NONE && !game.isDraw() && moveCount < 9) {
            auto availableMoves = game.getAvailableMoves();
            if (!availableMoves.empty()) {
                // Take a strategic move (prefer center, then corners, then edges)
                auto move = availableMoves[0];
                for (const auto& availableMove : availableMoves) {
                    // Prefer center
                    if (availableMove.first == 1 && availableMove.second == 1) {
                        move = availableMove;
                        break;
                    }
                    // Then prefer corners
                    if ((availableMove.first == 0 || availableMove.first == 2) &&
                        (availableMove.second == 0 || availableMove.second == 2)) {
                        move = availableMove;
                    }
                }
                
                std::string currentPlayerName = (game.getCurrentPlayer() == alicePlayer) ? "Alice" : "Bob";
                std::cout << currentPlayerName << " plays: (" << move.first << "," << move.second << ")" << std::endl;
                
                ASSERT_TRUE(game.makeMove(move.first, move.second));
                printGameBoard(game);
                moveCount++;
            }
        }
        
        // Determine results for both players
        std::string aliceResult, bobResult;
        Player winner = game.getWinner();
        
        if (winner == Player::NONE) {
            aliceResult = bobResult = "Draw";
        } else if (winner == alicePlayer) {
            aliceResult = "Win";
            bobResult = "Loss";
        } else {
            aliceResult = "Loss";
            bobResult = "Win";
        }
        
        std::cout << "Game " << gameNum << " result - Alice: " << aliceResult << ", Bob: " << bobResult << std::endl;
        
        // Save results
        std::string timestamp = getCurrentTimestamp();
        aliceHistory.saveResult({timestamp, aliceResult});
        bobHistory.saveResult({timestamp, bobResult});
    }
    
    // Verify both players have complete histories
    auto aliceHistoryData = aliceHistory.loadHistory();
    auto bobHistoryData = bobHistory.loadHistory();
    
    EXPECT_EQ(3, aliceHistoryData.size());
    EXPECT_EQ(3, bobHistoryData.size());
    
    // Count wins/losses/draws for verification
    int aliceWins = 0, aliceLosses = 0, aliceDraws = 0;
    int bobWins = 0, bobLosses = 0, bobDraws = 0;
    
    for (const auto& result : aliceHistoryData) {
        if (result.result == "Win") aliceWins++;
        else if (result.result == "Loss") aliceLosses++;
        else if (result.result == "Draw") aliceDraws++;
    }
    
    for (const auto& result : bobHistoryData) {
        if (result.result == "Win") bobWins++;
        else if (result.result == "Loss") bobLosses++;
        else if (result.result == "Draw") bobDraws++;
    }
    
    // Verify the results are consistent (Alice's wins = Bob's losses, etc.)
    EXPECT_EQ(aliceWins, bobLosses);
    EXPECT_EQ(aliceLosses, bobWins);
    EXPECT_EQ(aliceDraws, bobDraws);
    
    std::cout << "Alice: " << aliceWins << " wins, " << aliceLosses << " losses, " << aliceDraws << " draws" << std::endl;
    std::cout << "Bob: " << bobWins << " wins, " << bobLosses << " losses, " << bobDraws << " draws" << std::endl;
}

TEST_F(IntegrationTest, AITournament) {
    // Test AI vs AI games with different scenarios
    std::string dbFile = "tournament_users.db";
    addTestFile(dbFile);
    addTestFile("history_testplayer1.txt");
    addTestFile("history_testplayer2.txt");
    
    Auth auth(dbFile);
    auth.clearAllUsers();
    
    ASSERT_TRUE(auth.registerUser("testplayer1", "pass1"));
    ASSERT_TRUE(auth.registerUser("testplayer2", "pass2"));
    
    History player1History("testplayer1");
    History player2History("testplayer2");
    
    AI aiX(Player::X);
    AI aiO(Player::O);
    
    int totalGames = 5;
    int xWins = 0, oWins = 0, draws = 0;
    
    std::cout << "=== AI Tournament: " << totalGames << " games ===" << std::endl;
    
    for (int gameNum = 1; gameNum <= totalGames; gameNum++) {
        std::cout << "\n--- Game " << gameNum << " ---" << std::endl;
        
        Game game;
        int moveCount = 0;
        
        while (game.getWinner() == Player::NONE && !game.isDraw() && moveCount < 9) {
            if (game.getCurrentPlayer() == Player::X) {
                auto move = aiX.findBestMove(game);
                ASSERT_TRUE(game.makeMove(move.first, move.second));
                std::cout << "X plays: (" << move.first << "," << move.second << ")" << std::endl;
            } else {
                auto move = aiO.findBestMove(game);
                ASSERT_TRUE(game.makeMove(move.first, move.second));
                std::cout << "O plays: (" << move.first << "," << move.second << ")" << std::endl;
            }
            moveCount++;
        }
        
        printGameBoard(game);
        
        Player winner = game.getWinner();
        std::string result;
        
        if (winner == Player::X) {
            result = "X wins";
            xWins++;
        } else if (winner == Player::O) {
            result = "O wins";
            oWins++;
        } else {
            result = "Draw";
            draws++;
        }
        
        std::cout << "Result: " << result << std::endl;
        
        // Save to both players' histories
        std::string timestamp = getCurrentTimestamp();
        std::string player1Result = (winner == Player::X) ? "Win" : (winner == Player::O) ? "Loss" : "Draw";
        std::string player2Result = (winner == Player::O) ? "Win" : (winner == Player::X) ? "Loss" : "Draw";
        
        player1History.saveResult({timestamp, player1Result});
        player2History.saveResult({timestamp, player2Result});
    }
    
    std::cout << "\n=== Tournament Results ===" << std::endl;
    std::cout << "X wins: " << xWins << std::endl;
    std::cout << "O wins: " << oWins << std::endl;
    std::cout << "Draws: " << draws << std::endl;
    std::cout << "Total games: " << (xWins + oWins + draws) << std::endl;
    
    // With optimal play, all games should be draws
    EXPECT_EQ(totalGames, draws);
    EXPECT_EQ(0, xWins);
    EXPECT_EQ(0, oWins);
    
    // Verify histories
    auto player1HistoryData = player1History.loadHistory();
    auto player2HistoryData = player2History.loadHistory();
    
    EXPECT_EQ(totalGames, player1HistoryData.size());
    EXPECT_EQ(totalGames, player2HistoryData.size());
}

TEST_F(IntegrationTest, GameStateConsistency) {
    // Test game state consistency across multiple operations
    Game game;
    
    // Test initial state
    EXPECT_EQ(Player::X, game.getCurrentPlayer());
    EXPECT_EQ(Player::NONE, game.getWinner());
    EXPECT_FALSE(game.isDraw());
    EXPECT_EQ(9, game.getAvailableMoves().size());
    
    // Play a series of moves and verify state consistency
    std::vector<std::pair<int, int>> moves = {
        {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}
    };
    
    Player expectedPlayer = Player::X;
    int expectedAvailableMoves = 9;
    
    for (size_t i = 0; i < moves.size(); i++) {
        // Verify current player before move
        EXPECT_EQ(expectedPlayer, game.getCurrentPlayer());
        
        // Verify available moves count
        EXPECT_EQ(expectedAvailableMoves, game.getAvailableMoves().size());
        
        // Make move
        ASSERT_TRUE(game.makeMove(moves[i].first, moves[i].second));
        
        // Verify position is occupied
        EXPECT_EQ(expectedPlayer, game.at(moves[i].first, moves[i].second));
        
        // Update expected state
        expectedPlayer = (expectedPlayer == Player::X) ? Player::O : Player::X;
        expectedAvailableMoves--;
        
        // Check for win condition
        if (i == 5) { // After 6 moves, check if there's a winner
            Player winner = game.getWinner();
            if (winner != Player::NONE) {
                std::cout << "Game won by " << (winner == Player::X ? "X" : "O") << " after move " << (i + 1) << std::endl;
                break;
            }
        }
    }
    
    // Test reset functionality
    game.reset();
    
    EXPECT_EQ(Player::X, game.getCurrentPlayer());
    EXPECT_EQ(Player::NONE, game.getWinner());
    EXPECT_FALSE(game.isDraw());
    EXPECT_EQ(9, game.getAvailableMoves().size());
    
    // Verify all positions are empty
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(Player::NONE, game.at(i, j));
        }
    }
}

TEST_F(IntegrationTest, HistoryIntegrityWithMultipleUsers) {
    // Test that history files don't interfere with each other
    std::vector<std::string> users = {"player1", "player2"};
    std::vector<History> histories;
    
    // Setup histories for multiple users
    for (const auto& user : users) {
        addTestFile("history_" + user + ".txt");
        histories.emplace_back(user);
    }
    
    // Each user plays different numbers of games with different results
    std::vector<std::vector<GameResult>> userResults = {
        {
            {"2024-01-15 10:00:00", "Win"},
            {"2024-01-15 11:00:00", "Loss"},
            {"2024-01-15 12:00:00", "Draw"}
        },
        {
            {"2024-01-15 10:30:00", "Loss"},
            {"2024-01-15 11:30:00", "Win"},
            {"2024-01-15 12:30:00", "Win"},
            {"2024-01-15 13:30:00", "Draw"},
            {"2024-01-15 14:30:00", "Loss"}
        }
    };
    
    // Save results for each user
    for (size_t i = 0; i < users.size(); i++) {
        for (const auto& result : userResults[i]) {
            histories[i].saveResult(result);
        }
    }
    
    // Verify each user's history is independent and correct
    for (size_t i = 0; i < users.size(); i++) {
        auto loadedHistory = histories[i].loadHistory();
        
        EXPECT_EQ(userResults[i].size(), loadedHistory.size()) 
            << "History size mismatch for user " << users[i];
        
        for (size_t j = 0; j < std::min(userResults[i].size(), loadedHistory.size()); j++) {
            EXPECT_EQ(userResults[i][j].date, loadedHistory[j].date)
                << "Date mismatch for user " << users[i] << " at index " << j;
            EXPECT_EQ(userResults[i][j].result, loadedHistory[j].result)
                << "Result mismatch for user " << users[i] << " at index " << j;
        }
    }
    
    std::cout << "Verified independent histories for " << users.size() << " users" << std::endl;
}

TEST_F(IntegrationTest, StressTestAllComponents) {
    // Stress test all components working together
    std::string dbFile = "stress_test_users.db";
    addTestFile(dbFile);
    
    Auth auth(dbFile);
    auth.clearAllUsers();
    
    const int numUsers = 10;
    const int gamesPerUser = 5;
    
    std::cout << "=== Stress Test: " << numUsers << " users, " << gamesPerUser << " games each ===" << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now(); //Marks the start time to measure performance later
    
    // Register users
    for (int i = 1; i <= numUsers; i++) {
        std::string username = "stressuser" + std::to_string(i);
        std::string password = "pass" + std::to_string(i);
        
        addTestFile("history_" + username + ".txt");
        
        ASSERT_TRUE(auth.registerUser(username, password));
        ASSERT_TRUE(auth.loginUser(username, password));
    }
    
    // Each user plays games against AI
    AI ai(Player::O);
    
    for (int userId = 1; userId <= numUsers; userId++) {
        std::string username = "stressuser" + std::to_string(userId);
        History userHistory(username);
        
        for (int gameNum = 1; gameNum <= gamesPerUser; gameNum++) {
            Game game;
            
            // Quick game simulation
            while (game.getWinner() == Player::NONE && !game.isDraw()) {
                if (game.getCurrentPlayer() == Player::X) {
                    // Human player - take first available move
                    auto moves = game.getAvailableMoves();
                    if (!moves.empty()) {
                        game.makeMove(moves[0].first, moves[0].second);
                    }
                } else {
                    // AI player
                    auto aiMove = ai.findBestMove(game);
                    game.makeMove(aiMove.first, aiMove.second);
                }
            }
            
            // Save result
            std::string result = (game.getWinner() == Player::X) ? "Win" : 
                               (game.getWinner() == Player::O) ? "Loss" : "Draw";
            
            std::string timestamp = "2024-01-15 " + 
                std::to_string(10 + gameNum) + ":00:00";
            
            userHistory.saveResult({timestamp, result});
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    std::cout << "Stress test completed in " << duration.count() << "ms" << std::endl;
    std::cout << "Total operations: " << (numUsers * (1 + gamesPerUser)) << std::endl;
    std::cout << "Average time per operation: " << 
        (duration.count() / (numUsers * (1 + gamesPerUser))) << "ms" << std::endl;
    
    // Verify all users still exist and have correct history counts
    int totalHistoryEntries = 0;
    for (int userId = 1; userId <= numUsers; userId++) {
        std::string username = "stressuser" + std::to_string(userId);
        
        EXPECT_TRUE(auth.userExists(username));
        
        History userHistory(username);
        auto history = userHistory.loadHistory();
        
        EXPECT_EQ(gamesPerUser, history.size()) 
            << "Incorrect history size for " << username;
        
        totalHistoryEntries += history.size();
    }
    
    EXPECT_EQ(numUsers * gamesPerUser, totalHistoryEntries);
    
    std::cout << "Verified " << totalHistoryEntries << " total history entries across " 
              << numUsers << " users" << std::endl;
    
    // Performance check - should complete reasonably quickly
    EXPECT_LT(duration.count(), 10000); // Less than 10 seconds
}