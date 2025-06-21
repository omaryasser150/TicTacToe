// History.cpp placeholder
// History.cpp
#include "History.h"
#include <fstream>     // For reading/writing files (ifstream, ofstream)
#include <sstream>     // For parsing strings using stringstream

History::History(const std::string& username) {
    // Creates a filename like: history_omar.txt
    filename = "history_" + username + ".txt";
}


void History::saveResult(const GameResult& res) {
    std::ofstream out(filename, std::ios::app);  // Open file in append mode
    out << res.date << "," << res.result << "\n";  // Save result as: date,result
}


std::vector<GameResult> History::loadHistory() const {
    std::ifstream in(filename);         // Open file for reading
    std::string line;                   // Line buffer for reading file line by line
    std::vector<GameResult> history;    // Vector to hold all parsed game results

    while (std::getline(in, line)) {
        std::stringstream ss(line);     // Used to split line into parts
        std::string date, result;

        // Split the line into 'date' and 'result' using comma
        if (std::getline(ss, date, ',') && std::getline(ss, result)) {
            history.push_back({date, result});  // Add parsed result to vector
        }
    }

    return history;  // Return the full history as a vector
}
