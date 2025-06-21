#ifndef HISTORY_H
#define HISTORY_H

#include <string>   
#include <vector>   

// Struct to represent one game result (e.g., "2025-06-18", "X won")
struct GameResult {
    std::string date;    // The date when the game was played
    std::string result;  // The result: "X won", "O won", "Draw", etc.
};

// Class to manage saving/loading history to/from a file
class History {
private:
    std::string filename;  // The name of the file where this user's history is stored

public:
    // Constructor: sets the filename based on username
    History(const std::string& username);

    // Appends a single result to the history file
    void saveResult(const GameResult& res);

    // Loads all results from the history file into a vector
    std::vector<GameResult> loadHistory() const;
};

#endif
