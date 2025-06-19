// Auth.cpp
#include "Auth.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <functional>
#include <chrono>

Auth::Auth(const std::string& filename) : dbFile(filename) {
    loadUsers();
}

std::string Auth::hashPassword(const std::string& password, const std::string& salt) {
    // Use application-wide salt if no specific salt provided
    const std::string defaultSalt = "MyApp2024SecureSalt!@#";
    std::string actualSalt = salt.empty() ? defaultSalt : salt;

    // Create salted password
    std::string saltedPassword = actualSalt + password + actualSalt;

    // Use std::hash for initial hash
    std::hash<std::string> hasher;
    size_t hash1 = hasher(saltedPassword);

    // Double hash for extra security
    std::string firstHashStr = std::to_string(hash1) + actualSalt;
    size_t hash2 = hasher(firstHashStr);

    // Convert to hex string
    std::stringstream ss;
    ss << std::hex << hash2;
    return ss.str();
}

std::string Auth::generateRandomSalt() {
    // Use current time and random device for seed
    std::random_device rd;
    std::mt19937 gen(rd() ^ std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dis(0, 255);

    std::stringstream ss;
    // Generate 16 random bytes as hex
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << dis(gen);
    }
    return ss.str();
}

void Auth::loadUsers() {
    users.clear();
    std::ifstream inFile(dbFile);
    if (!inFile.is_open()) {
        // File doesn't exist yet, that's okay
        return;
    }

    std::string line;
    while (getline(inFile, line)) {
        size_t sep = line.find(':');
        if (sep != std::string::npos) {
            std::string username = line.substr(0, sep);
            std::string data = line.substr(sep + 1);
            users[username] = data;
        }
    }
    inFile.close();
}

void Auth::saveUsers() {
    std::ofstream outFile(dbFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << dbFile << std::endl;
        return;
    }

    for (const auto& [username, data] : users) {
        outFile << username << ":" << data << "\n";
    }
    outFile.close();
}

bool Auth::registerUser(const std::string& username, const std::string& password) {
    // Check if username already exists
    if (users.count(username)) {
        return false;
    }

    // Validate input
    if (username.empty() || password.empty()) {
        return false;
    }

    // Check for invalid characters in username (no colons allowed)
    if (username.find(':') != std::string::npos) {
        return false;
    }

    // Generate unique salt for this user
    std::string salt = generateRandomSalt();

    // Hash password with salt
    std::string hashedPassword = hashPassword(password, salt);

    // Store as "salt:hash"
    users[username] = salt + ":" + hashedPassword;

    // Save to file
    saveUsers();

    std::cout << "User '" << username << "' registered successfully." << std::endl;
    return true;
}

bool Auth::loginUser(const std::string& username, const std::string& password) {
    // Check if user exists
    if (!users.count(username)) {
        return false;
    }

    std::string storedData = users[username];

    // Check if this is old format (no salt) or new format (with salt)
    size_t sep = storedData.find(':');

    if (sep == std::string::npos) {
        // Old format - just hash, no salt
        // For backward compatibility, use default salt
        std::string computedHash = hashPassword(password);
        return computedHash == storedData;
    } else {
        // New format - salt:hash
        std::string salt = storedData.substr(0, sep);
        std::string storedHash = storedData.substr(sep + 1);

        // Compute hash with stored salt
        std::string computedHash = hashPassword(password, salt);

        if (computedHash == storedHash) {
            std::cout << "User '" << username << "' logged in successfully." << std::endl;
            return true;
        }
    }

    return false;
}

bool Auth::userExists(const std::string& username) {
    return users.count(username) > 0;
}

bool Auth::changePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    // Verify old password first
    if (!loginUser(username, oldPassword)) {
        return false;
    }

    // Generate new salt and hash
    std::string newSalt = generateRandomSalt();
    std::string newHashedPassword = hashPassword(newPassword, newSalt);

    // Update stored data
    users[username] = newSalt + ":" + newHashedPassword;

    // Save to file
    saveUsers();

    std::cout << "Password changed successfully for user '" << username << "'." << std::endl;
    return true;
}

void Auth::clearAllUsers() {
    users.clear();
    saveUsers();
    std::cout << "All users cleared." << std::endl;
}
