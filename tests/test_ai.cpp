// test_ai.cpp
#include "AI.h"
#include "Game.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>

class AITester {
private:
    int testsPassed = 0;
    int totalTests = 0;

    void assert_test(bool condition, const std::string& testName) {
        totalTests++;
        if (condition) {
            testsPassed++;
            std::cout << "[PASS] " << testName << std::endl;
        } else {
            std::cout << "[FAIL] " << testName << std::endl;
        }
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

public:
    void testAIInitialization() {
        std::cout << "\n=== Testing AI Initialization ===" << std::endl;
        
        // Test AI initialization with Player X
        AI aiX(Player::X);
        assert_test(true, "AI initialized with Player X");
        
        // Test AI initialization with Player O
        AI aiO(Player::O);
        assert_test(true, "AI initialized with Player O");
    }

    void testAIWinningMove() {
        std::cout << "\n=== Testing AI Winning Moves ===" << std::endl;
        
        // Test case 1: AI can win in one move (horizontal)
        Game game1;
        game1.makeMove(0, 0); // X
        game1.makeMove(1, 0); // O
        game1.makeMove(0, 1); // X
        game1.makeMove(1, 1); // O
        // Board: X X .
        //        O O .
        //        . . .
        // X should play (0,2) to win
        
        AI aiX(Player::X);
        auto move1 = aiX.findBestMove(game1);
        assert_test(move1.first == 0 && move1.second == 2, "AI finds winning move (horizontal)");
        
        // Test case 2: AI can win in one move (vertical)
        Game game2;
        game2.makeMove(0, 0); // X
        game2.makeMove(0, 1); // O
        game2.makeMove(1, 0); // X
        game2.makeMove(0, 2); // O
        // Board: X O O
        //        X . .
        //        . . .
        // X should play (2,0) to win
        
        auto move2 = aiX.findBestMove(game2);
        assert_test(move2.first == 2 && move2.second == 0, "AI finds winning move (vertical)");
        
        // Test case 3: AI can win in one move (diagonal)
        Game game3;
        game3.makeMove(0, 0); // X
        game3.makeMove(0, 1); // O
        game3.makeMove(1, 1); // X
        game3.makeMove(0, 2); // O
        // Board: X O O
        //        . X .
        //        . . .
        // X should play (2,2) to win
        
        auto move3 = aiX.findBestMove(game3);
        assert_test(move3.first == 2 && move3.second == 2, "AI finds winning move (diagonal)");
    }

    void testAIBlockingMove() {
        std::cout << "\n=== Testing AI Blocking Moves ===" << std::endl;
        
        // Test case 1: AI blocks opponent's winning move (horizontal)
        Game game1;
        game1.makeMove(0, 0); // X
        game1.makeMove(1, 0); // O
        game1.makeMove(0, 1); // X
        game1.makeMove(1, 1); // O
        game1.makeMove(2, 0); // X
        // Board: X X .
        //        O O .
        //        X . .
        // O should play (0,2) to block X's win
        
        AI aiO(Player::O);
        auto move1 = aiO.findBestMove(game1);
        assert_test(move1.first == 0 && move1.second == 2, "AI blocks opponent's winning move (horizontal)");
        
        // Test case 2: AI blocks opponent's winning move (diagonal)
        Game game2;
        game2.makeMove(0, 0); // X
        game2.makeMove(0, 1); // O
        game2.makeMove(1, 1); // X
        game2.makeMove(0, 2); // O
        game2.makeMove(1, 0); // X
        // Board: X O O
        //        X X .
        //        . . .
        // O should play (2,2) to block X's diagonal win
        
        auto move2 = aiO.findBestMove(game2);
        assert_test(move2.first == 2 && move2.second == 2, "AI blocks opponent's winning move (diagonal)");
    }

    void testAIOptimalPlay() {
        std::cout << "\n=== Testing AI Optimal Play ===" << std::endl;
        
        // Test case 1: AI takes center when available
        Game game1;
        game1.makeMove(0, 0); // X
        game1.makeMove(2, 2); // O (AI's turn would be X now)
        // Board: X . .
        //        . . .
        //        . . O
        // X should prefer center (1,1) as it's optimal
        
        AI aiX(Player::X);
        auto move1 = aiX.findBestMove(game1);
        assert_test(move1.first == 1 && move1.second == 1, "AI takes center when available");
        
        // Test case 2: AI makes strategic corner move
        Game game2;
        game2.makeMove(1, 1); // X takes center
        // Board: . . .
        //        . X .
        //        . . .
        // O should take a corner
        
        AI aiO(Player::O);
        auto move2 = aiO.findBestMove(game2);
        bool isCorner = (move2.first == 0 && move2.second == 0) ||
                       (move2.first == 0 && move2.second == 2) ||
                       (move2.first == 2 && move2.second == 0) ||
                       (move2.first == 2 && move2.second == 2);
        assert_test(isCorner, "AI takes corner when opponent has center");
    }

    void testAIDrawScenario() {
        std::cout << "\n=== Testing AI Draw Scenarios ===" << std::endl;
        
        // Test case: AI plays optimally to force a draw
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
        assert_test(move.first == 2 && move.second == 1, "AI makes final move in draw scenario");
        
        // Complete the game and verify it's a draw
        game.makeMove(move.first, move.second);
        assert_test(game.isDraw(), "Game results in draw with optimal play");
    }

    void testAIInvalidMoves() {
        std::cout << "\n=== Testing AI with Invalid Scenarios ===" << std::endl;
        
        // Test case: AI doesn't return invalid moves
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
        assert_test(validMove, "AI returns valid move");
        
        // Verify the move is one of the available moves
        auto availableMoves = game.getAvailableMoves();
        bool foundMove = false;
        for (const auto& availableMove : availableMoves) {
            if (availableMove.first == move.first && availableMove.second == move.second) {
                foundMove = true;
                break;
            }
        }
        assert_test(foundMove, "AI returns move from available moves");
    }

    void testAIGameEndStates() {
        std::cout << "\n=== Testing AI with Game End States ===" << std::endl;
        
        // Test case: AI behavior when game is already won
        Game wonGame;
        wonGame.makeMove(0, 0); // X
        wonGame.makeMove(1, 0); // O
        wonGame.makeMove(0, 1); // X
        wonGame.makeMove(1, 1); // O
        wonGame.makeMove(0, 2); // X wins
        // Board: X X X
        //        O O .
        //        . . .
        
        assert_test(wonGame.getWinner() == Player::X, "Game is won by X");
        
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
        assert_test(validMove || availableMoves.empty(), "AI handles won game state");
    }

    void testAIPerformance() {
        std::cout << "\n=== Testing AI Performance ===" << std::endl;
        
        // Test case: AI completes move calculation in reasonable time
        Game game;
        AI ai(Player::X);
        
        auto start = std::chrono::high_resolution_clock::now();
        auto move = ai.findBestMove(game);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "AI calculation time: " << duration.count() << "ms" << std::endl;
        assert_test(duration.count() < 5000, "AI completes calculation in under 5 seconds");
        assert_test(move.first >= 0 && move.first < 3 && move.second >= 0 && move.second < 3, 
                   "AI returns valid coordinates");
    }

    void runAllTests() {
        std::cout << "Starting AI Tests..." << std::endl;
        std::cout << "===================" << std::endl;
        
        testAIInitialization();
        testAIWinningMove();
        testAIBlockingMove();
        testAIOptimalPlay();
        testAIDrawScenario();
        testAIInvalidMoves();
        testAIGameEndStates();
        testAIPerformance();
        
        std::cout << "\n===================" << std::endl;
        std::cout << "AI Tests Summary:" << std::endl;
        std::cout << "Passed: " << testsPassed << "/" << totalTests << std::endl;
        std::cout << "Success Rate: " << (totalTests > 0 ? (testsPassed * 100.0 / totalTests) : 0) << "%" << std::endl;
        
        if (testsPassed == totalTests) {
            std::cout << "All AI tests passed! ✓" << std::endl;
        } else {
            std::cout << "Some AI tests failed! ✗" << std::endl;
        }
    }
};

// Function to be called from main test runner
void runAITests() {
    AITester tester;
    tester.runAllTests();
}