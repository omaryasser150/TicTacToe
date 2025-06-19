// Auth.h
#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <unordered_map>

class Auth {
private:
    std::string dbFile;
    std::unordered_map<std::string, std::string> users;

    void loadUsers();
    void saveUsers();
    std::string hashPassword(const std::string& password, const std::string& salt = "");
    std::string generateRandomSalt();

public:
    Auth(const std::string& filename = "users.db");
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);

    // Optional: methods to check user existence, change password, etc.
    bool userExists(const std::string& username);
    bool changePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
    void clearAllUsers(); // For testing purposes
};

#endif
