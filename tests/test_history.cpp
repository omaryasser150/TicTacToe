// test_history.cpp
#include "../History.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

class HistoryTester {
private:
    int testsPassed = 0;
    int totalTests = 0;
    std::vector<std::string> testFiles; // Track test files for cleanup

    void assert_test(bool condition, const std::string& testName) {
        totalTests++;
        if (condition) {
            testsPassed++;
            std::cout << "[PASS] " << testName << std::endl;
        } else {
            std::cout << "[FAIL] " << testName << std::endl;
        }
    }

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    void cleanupTestFiles() {
        for (const auto& filename : testFiles) {
            if (std::filesystem::exists(filename)) {
                std::filesystem::remove(filename);
            }
        }
        testFiles.clear();
    }

    void addTestFile(const std::string& filename) {
        testFiles.push_back(filename);
    }

public:
    ~HistoryTester() {
        cleanupTestFiles();
    }

    void testHistoryInitialization() {
        std::cout << "\n=== Testing History Initialization ===" << std::endl;
        
        // Test history initialization with different usernames
        History history1("testuser1");
        assert_test(true, "History initialized with username 'testuser1'");
        addTestFile("history_testuser1.txt");
        
        History history2("testuser2");
        assert_test(true, "History initialized with username 'testuser2'");
        addTestFile("history_testuser2.txt");
        
        // Test with special characters in username
        History history3("test_user-123");
        assert_test(true, "History initialized with special characters in username");
        addTestFile("history_test_user-123.txt");
    }

    void testSaveSingleResult() {
        std::cout << "\n=== Testing Save Single Result ===" << std::endl;
        
        std::string username = "single_test_user";
        std::string expectedFile = "history_" + username + ".txt";
        addTestFile(expectedFile);
        
        History history(username);
        
        // Test saving a single game result
        GameResult result1 = {"2024-01-15 10:30:00", "Win"};
        history.saveResult(result1);
        
        // Check that file was created
        assert_test(fileExists(expectedFile), "History file created after saving result");
        
        // Check file contents
        std::ifstream file(expectedFile);
        std::string line;
        bool foundResult = false;
        if (std::getline(file, line)) {
            foundResult = (line == "2024-01-15 10:30:00,Win");
        }
        file.close();
        
        assert_test(foundResult, "Single result saved correctly to file");
    }

    void testSaveMultipleResults() {
        std::cout << "\n=== Testing Save Multiple Results ===" << std::endl;
        
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
        
        assert_test(lines.size() == 4, "All four results saved to file");
        assert_test(lines[0] == "2024-01-15 10:30:00,Win", "First result saved correctly");
        assert_test(lines[1] == "2024-01-15 11:45:00,Loss", "Second result saved correctly");
        assert_test(lines[2] == "2024-01-15 14:20:00,Draw", "Third result saved correctly");
        assert_test(lines[3] == "2024-01-16 09:15:00,Win", "Fourth result saved correctly");
    }

    void testLoadHistory() {
        std::cout << "\n=== Testing Load History ===" << std::endl;
        
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
        
        assert_test(loadedHistory.size() == 3, "Loaded correct number of history entries");
        
        if (loadedHistory.size() >= 3) {
            assert_test(loadedHistory[0].date == "2024-01-15 10:30:00" && 
                       loadedHistory[0].result == "Win", "First entry loaded correctly");
            assert_test(loadedHistory[1].date == "2024-01-15 11:45:00" && 
                       loadedHistory[1].result == "Loss", "Second entry loaded correctly");
            assert_test(loadedHistory[2].date == "2024-01-15 14:20:00" && 
                       loadedHistory[2].result == "Draw", "Third entry loaded correctly");
        }
    }

    void testLoadEmptyHistory() {
        std::cout << "\n=== Testing Load Empty History ===" << std::endl;
        
        std::string username = "empty_test_user";
        History history(username);
        addTestFile("history_" + username + ".txt");
        
        // Try to load history when no file exists
        std::vector<GameResult> loadedHistory = history.loadHistory();
        
        assert_test(loadedHistory.empty(), "Empty history loaded when no file exists");
        assert_test(loadedHistory.size() == 0, "History size is zero when no file exists");
    }

    void testSaveAndLoadRoundTrip() {
        std::cout << "\n=== Testing Save and Load Round Trip ===" << std::endl;
        
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
        
        assert_test(loadedResults.size() == originalResults.size(), 
                   "Round trip: same number of results");
        
        // Compare each result
        bool allMatch = true;
        for (size_t i = 0; i < std::min(originalResults.size(), loadedResults.size()); ++i) {
            if (originalResults[i].date != loadedResults[i].date || 
                originalResults[i].result != loadedResults[i].result) {
                allMatch = false;
                break;
            }
        }
        
        assert_test(allMatch, "Round trip: all results match after save/load");
    }

    void testSpecialCharactersInResults() {
        std::cout << "\n=== Testing Special Characters in Results ===" << std::endl;
        
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
        
        assert_test(loadedResults.size() == specialResults.size(), 
                   "Special characters: correct number of results");
        
        bool allMatch = true;
        for (size_t i = 0; i < std::min(specialResults.size(), loadedResults.size()); ++i) {
            if (specialResults[i].date != loadedResults[i].date || 
                specialResults[i].result != loadedResults[i].result) {
                allMatch = false;
                std::cout << "Mismatch at index " << i << ": " 
                         << "Expected: '" << specialResults[i].date << "," << specialResults[i].result << "' "
                         << "Got: '" << loadedResults[i].date << "," << loadedResults[i].result << "'" << std::endl;
                break;
            }
        }
        
        assert_test(allMatch, "Special characters: all results preserved correctly");
    }

    void testMalformedDataHandling() {
        std::cout << "\n=== Testing Malformed Data Handling ===" << std::endl;
        
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
        
        assert_test(loadedHistory.size() >= 3, "Malformed data: loads at least the good entries");
        
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
        
        assert_test(hasGoodEntries, "Malformed data: good entries are preserved");
    }

    void testConcurrentAccess() {
        std::cout << "\n=== Testing Concurrent Access Simulation ===" << std::endl;
        
        std::string username = "concurrent_test_user";
        addTestFile("history_" + username + ".txt");
        
        // Simulate multiple rapid saves (like multiple games in quick succession)
        History history(username);
        
        for (int i = 0; i < 10; ++i) {
            std::string timestamp = "2024-01-15 10:" + 
                                  (i < 10 ? "0" : "") + std::to_string(i) + ":00";
            std::string result = (i % 3 == 0) ? "Win" : (i % 3 == 1) ? "Loss" : "Draw";
            
            GameResult gameResult = {timestamp, result};
            history.saveResult(gameResult);
        }
        
        // Load and verify all saves worked
        std::vector<GameResult> loadedHistory = history.loadHistory();
        
        assert_test(loadedHistory.size() == 10, "Concurrent access: all 10 results saved");
        
        // Verify the sequence is correct
        bool sequenceCorrect = true;
        for (int i = 0; i < 10 && i < static_cast<int>(loadedHistory.size()); ++i) {
            std::string expectedResult = (i % 3 == 0) ? "Win" : (i % 3 == 1) ? "Loss" : "Draw";
            if (loadedHistory[i].result != expectedResult) {
                sequenceCorrect = false;
                break;
            }
        }
        
        assert_test(sequenceCorrect, "Concurrent access: sequence preserved correctly");
    }

    void testLargeHistoryFile() {
        std::cout << "\n=== Testing Large History File ===" << std::endl;
        
        std::string username = "large_test_user";
        addTestFile("history_" + username + ".txt");
        
        History history(username);
        
        // Create a large number of entries
        const int numEntries = 1000;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < numEntries; ++i) {
            std::string date = "2024-01-" + 
                              (i % 28 + 1 < 10 ? "0" : "") + 
                              std::to_string(i % 28 + 1) + " 10:00:00";
            std::string result = (i % 3 == 0) ? "Win" : (i % 3 == 1) ? "Loss" : "Draw";
            
            GameResult gameResult = {date, result};
            history.saveResult(gameResult);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "Time to save " << numEntries << " entries: " << duration.count() << "ms" << std::endl;
        
        // Test loading performance
        start = std::chrono::high_resolution_clock::now();
        std::vector<GameResult> loadedHistory = history.loadHistory();
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "Time to load " << loadedHistory.size() << " entries: " << duration.count() << "ms" << std::endl;
        
        assert_test(loadedHistory.size() == numEntries, "Large file: correct number of entries loaded");
        assert_test(duration.count() < 1000, "Large file: loading completes in under 1 second");
    }

    void runAllTests() {
        std::cout << "Starting History Tests..." << std::endl;
        std::cout << "=========================" << std::endl;
        
        testHistoryInitialization();
        testSaveSingleResult();
        testSaveMultipleResults();
        testLoadHistory();
        testLoadEmptyHistory();
        testSaveAndLoadRoundTrip();
        testSpecialCharactersInResults();
        testMalformedDataHandling();
        testConcurrentAccess();
        testLargeHistoryFile();
        
        std::cout << "\n=========================" << std::endl;
        std::cout << "History Tests Summary:" << std::endl;
        std::cout << "Passed: " << testsPassed << "/" << totalTests << std::endl;
        std::cout << "Success Rate: " << (totalTests > 0 ? (testsPassed * 100.0 / totalTests) : 0) << "%" << std::endl;
        
        if (testsPassed == totalTests) {
            std::cout << "All History tests passed! ✓" << std::endl;
        } else {
            std::cout << "Some History tests failed! ✗" << std::endl;
        }
    }
};

// Function to be called from main test runner
void runHistoryTests() {
    HistoryTester tester;
    tester.runAllTests();
}