// History.cpp placeholder
// History.cpp
#include "History.h"
#include <fstream>
#include <sstream>

History::History(const std::string& username) {
    filename = "history_" + username + ".txt";
}

void History::saveResult(const GameResult& res) {
    std::ofstream out(filename, std::ios::app);
    out << res.date << "," << res.result << "\n";
}

std::vector<GameResult> History::loadHistory() const {
    std::ifstream in(filename);
    std::string line;
    std::vector<GameResult> history;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string date, result;
        if (std::getline(ss, date, ',') && std::getline(ss, result)) {
            history.push_back({date, result});
        }
    }

    return history;
}
