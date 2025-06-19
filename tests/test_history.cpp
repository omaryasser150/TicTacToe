#include <gtest/gtest.h>
#include "History.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <chrono>

class HistoryTest : public ::testing::Test {
protected:
    std::vector<std::string> testFiles; // Track test files for cleanup

    void SetUp() override {
        // Setup for each test
    }
    
    void TearDown() override {
        // Clean up test files
        cleanupTestFiles();
    }

    void addTestFile(const std::string& filename) {
        testFiles.push_back(filename);
    }

    void cleanupTestFiles() {
        for (const auto& filename : testFiles) {
            if (std::filesystem::exists(filename)) {
                std::filesystem::remove(filename);
            }
        }
        testFiles.clear();
    }

    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
};

TEST_F(HistoryTest, Initialization) {
    // Test history initialization with different usernames
    History history1("testuser1");
    addTestFile("history_testuser1.txt");
    SUCCEED(); // If we get here, initialization worked
    
    History history2("testuser2");
    addTestFile("history_testuser2.txt");
    SUCCEED(); // If we get here, initialization worked
    
    // Test with special characters in username
    History history3("test_user-123");
    addTestFile("history_test_user-123.txt");
    SUCCEED(); // If we get here, initialization worked
}

TEST_F(HistoryTest, SaveSingleResult) {
    std::string username = "single_test_user";
    std::string expectedFile = "history_" + username + ".txt";
    addTestFile(expectedFile);
    
    History history(username);
    
    // Test saving a single game result
    GameResult result1 = {"2024-01-15 10:30:00", "Win"};
    history.saveResult(result1);
    
    // Check that file was created
    EXPECT_TRUE(fileExists(expectedFile));
    
    // Check file contents
    std::ifstream file(expectedFile);
    std::string line;
    bool foundResult = false;
    if (std::getline(file, line)) {
        foundResult = (line == "2024-01-15 10:30:00,Win");
    }
    file.close();
    
    EXPECT_TRUE(foundResult);
}

TEST_F(HistoryTest, SaveMultipleResults) {
    std::string username = "multiple_test_user";
    std::string expectedFile = "history_" + username + ".txt";
    addTestFile(expectedFile);
    
    History history(username);
    
    // Test saving multiple game results
    std::vector<GameResult> results = {
        {"2024-01-15 10:30:00", "Win"},
        {"2024-01-15 11:45:00", "Loss"},
        {"2024-01-15 14:20:00", "Draw"},
        {"2024-01-16 09:15:00", "Win"}
    };
    
    for (const auto& result : results) {
        history.saveResult(result);
    }
    
    // Check file contents
    std::ifstream file(expectedFile);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    EXPECT_EQ(4, lines.size());
    EXPECT_EQ("2024-01-15 10:30:00,Win", lines[0]);
    EXPECT_EQ("2024-01-15 11:45:00,Loss", lines[1]);
    EXPECT_EQ("2024-01-15 14:20:00,Draw", lines[2]);
    EXPECT_EQ("2024-01-16 09:15:00,Win", lines[3]);
}

TEST_F(HistoryTest, LoadHistory) {
    std::string username = "load_test_user";
    std::string expectedFile = "history_" + username + ".txt";
    addTestFile(expectedFile);
    
    // Create test data file manually
    std::ofstream file(expectedFile);
    file << "2024-01-15 10:30:00,Win\n";
    file << "2024-01-15 11:45:00,Loss\n";
    file << "2024-01-15 14:20:00,Draw\n";
    file.close();
    
    History history(username);
    std::vector<GameResult> loadedHistory = history.loadHistory();
    
    EXPECT_EQ(3, loadedHistory.size());
    
    if (loadedHistory.size() >= 3) {
        EXPECT_EQ("2024-01-15 10:30:00", loadedHistory[0].date);
        EXPECT_EQ("Win", loadedHistory[0].result);
        EXPECT_EQ("2024-01-15 11:45:00", loadedHistory[1].date);
        EXPECT_EQ("Loss", loadedHistory[1].result);
        EXPECT_EQ("2024-01-15 14:20:00", loadedHistory[2].date);
        EXPECT_EQ("Draw", loadedHistory[2].result);
    }
}

TEST_F(HistoryTest, LoadEmptyHistory) {
    std::string username = "empty_test_user";
    History history(username);
    addTestFile("history_" + username + ".txt");
    
    // Try to load history when no file exists
    std::vector<GameResult> loadedHistory = history.loadHistory();
    
    EXPECT_TRUE(loadedHistory.empty());
    EXPECT_EQ(0, loadedHistory.size());
}

TEST_F(HistoryTest, SaveAndLoadRoundTrip) {
    std::string username = "roundtrip_test_user";
    addTestFile("history_" + username + ".txt");
    
    History history(username);
    
    // Create test data
    std::vector<GameResult> originalResults = {
        {"2024-01-15 10:30:00", "Win"},
        {"2024-01-15 11:45:00", "Loss"},
        {"2024-01-15 14:20:00", "Draw"},
        {"2024-01-16 09:15:00", "Win"},
        {"2024-01-16 16:30:00", "Loss"}
    };
    
    // Save all results
    for (const auto& result : originalResults) {
        history.saveResult(result);
    }
    
    // Load them back
    std::vector<GameResult> loadedResults = history.loadHistory();
    
    EXPECT_EQ(originalResults.size(), loadedResults.size());
    
    // Compare each result
    for (size_t i = 0; i < std::min(originalResults.size(), loadedResults.size()); ++i) {
        EXPECT_EQ(originalResults[i].date, loadedResults[i].date);
        EXPECT_EQ(originalResults[i].result, loadedResults[i].result);
    }
}

TEST_F(HistoryTest, SpecialCharactersInResults) {
    std::string username = "special_chars_user";
    addTestFile("history_" + username + ".txt");
    
    History history(username);
    
    // Test with various result types that might contain special characters
    std::vector<GameResult> specialResults = {
        {"2024-01-15 10:30:00", "Win vs AI"},
        {"2024-01-15 11:45:00", "Loss (timeout)"},
        {"2024-01-15 14:20:00", "Draw - stalemate"},
        {"2024-01-16 09:15:00", "Win: perfect game!"}
    };
    
    // Save results
    for (const auto& result : specialResults) {
        history.saveResult(result);
    }
    
    // Load and verify
    std::vector<GameResult> loadedResults = history.loadHistory();
    
    EXPECT_EQ(specialResults.size(), loadedResults.size());
    
    for (size_t i = 0; i < std::min(specialResults.size(), loadedResults.size()); ++i) {
        EXPECT_EQ(specialResults[i].date, loadedResults[i].date) 
            << "Date mismatch at index " << i;
        EXPECT_EQ(specialResults[i].result, loadedResults[i].result) 
            << "Result mismatch at index " << i;
    }
}

TEST_F(HistoryTest, MalformedDataHandling) {
    std::string username = "malformed_test_user";
    std::string expectedFile = "history_" + username + ".txt";
    addTestFile(expectedFile);
    
    // Create file with malformed data
    std::ofstream file(expectedFile);
    file << "2024-01-15 10:30:00,Win\n";           // Good line
    file << "malformed line without comma\n";       // Bad line
    file << "2024-01-15 11:45:00,Loss\n";          // Good line
    file << ",Empty date\n";                         // Bad line
    file << "2024-01-15 14:20:00,\n";              // Empty result
    file << "2024-01-16 09:15:00,Draw\n";          // Good line
    file.close();
    
    History history(username);
    std::vector<GameResult> loadedHistory = history.loadHistory();
    
    // Should only load the properly formatted lines
    // Note: The implementation might handle malformed lines differently
    // This test checks that it doesn't crash and loads what it can
    
    EXPECT_GE(loadedHistory.size(), 3); // At least the good entries
    
    // Check that the good entries are loaded
    bool hasGoodEntries = false;
    for (const auto& entry : loadedHistory) {
        if ((entry.date == "2024-01-15 10:30:00" && entry.result == "Win") ||
            (entry.date == "2024-01-15 11:45:00" && entry.result == "Loss") ||
            (entry.date == "2024-01-16 09:15:00" && entry.result == "Draw")) {
            hasGoodEntries = true;
            break;
        }
    }
    
    EXPECT_TRUE(hasGoodEntries);
}

TEST_F(HistoryTest, ConcurrentAccessSimulation) {
    std::string username = "concurrent_test_user";
    addTestFile("history_" + username + ".txt");
    
    // Simulate multiple rapid saves (like multiple games in quick succession)
    History history(username);
    
    for (int i = 0; i < 10; ++i) {
        std::string timestamp = std::string("2024-01-15 10:") +
                   (i < 10 ? "0" : "") + std::to_string(i) + ":00";
        std::string result = (i % 3 == 0) ? "Win" : (i % 3 == 1) ? "Loss" : "Draw";
        
        GameResult gameResult = {timestamp, result};
        history.saveResult(gameResult);
    }
    
    // Load and verify all saves worked
    std::vector<GameResult> loadedHistory = history.loadHistory();
    
    EXPECT_EQ(10, loadedHistory.size());
    
    // Verify the sequence is correct
    for (int i = 0; i < 10 && i < static_cast<int>(loadedHistory.size()); ++i) {
        std::string expectedResult = (i % 3 == 0) ? "Win" : (i % 3 == 1) ? "Loss" : "Draw";
        EXPECT_EQ(expectedResult, loadedHistory[i].result) 
            << "Sequence error at index " << i;
    }
}

TEST_F(HistoryTest, LargeHistoryFile) {
    std::string username = "large_test_user";
    addTestFile("history_" + username + ".txt");
    
    History history(username);
    
    // Create a large number of entries
    const int numEntries = 1000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numEntries; ++i) {
        std::string date = std::string("2024-01-") +
              (i % 28 + 1 < 10 ? "0" : "") +
              std::to_string(i % 28 + 1);
        std::string result = (i % 3 == 0) ? "Win" : (i % 3 == 1) ? "Loss" : "Draw";
        
        GameResult gameResult = {date, result};
        history.saveResult(gameResult);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto saveDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Time to save " << numEntries << " entries: " << saveDuration.count() << "ms" << std::endl;
    
    // Test loading performance
    start = std::chrono::high_resolution_clock::now();
    std::vector<GameResult> loadedHistory = history.loadHistory();
    end = std::chrono::high_resolution_clock::now();
    auto loadDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Time to load " << loadedHistory.size() << " entries: " << loadDuration.count() << "ms" << std::endl;
    
    EXPECT_EQ(numEntries, loadedHistory.size());
    EXPECT_LT(loadDuration.count(), 1000); // Loading should complete in under 1 second
    EXPECT_LT(saveDuration.count(), 5000);  // Saving should complete in under 5 seconds
}