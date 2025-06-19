#include <iostream>
#include <functional>
#include <vector>
#include <string>

class TestRunner {
private:
    std::vector<std::pair<std::string, std::function<void()>>> tests;
    int passed = 0;
    int failed = 0;

public:
    void addTest(const std::string& name, std::function<void()> test) {
        tests.push_back({name, test});
    }

    void runAll() {
        std::cout << "Running " << tests.size() << " tests...\n" << std::endl;
        
        for (const auto& [name, test] : tests) {
            try {
                test();
                std::cout << "[PASS] " << name << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cout << "[FAIL] " << name << " - " << e.what() << std::endl;
                failed++;
            }
        }
        
        std::cout << "\nResults: " << passed << " passed, " << failed << " failed" << std::endl;
    }
};

TestRunner testRunner;

// Declare test functions
void runAuthTests();
void runGameTests();
void runAITests();

int main() {
    runAuthTests();
    runGameTests();
    runAITests();
    
    testRunner.runAll();
    return 0;
}