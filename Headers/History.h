// History.h placeholder
// History.h
#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <vector>

struct GameResult {
    std::string date;
    std::string result;
};

class History {
private:
    std::string filename;

public:
    History(const std::string& username);
    void saveResult(const GameResult& res);
    std::vector<GameResult> loadHistory() const;
};

#endif
