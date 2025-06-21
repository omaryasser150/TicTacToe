// Auth.h
#ifndef AUTH_H
#define AUTH_H

#include <string>               // For username/password strings
#include <unordered_map>        // For fast username-to-password lookup

class Auth {
private:
    std::string dbFile;         // The name of the file where user data is saved
    std::unordered_map<std::string, std::string> users; // In-memory map: username → salt:hashedPassword

    void loadUsers();           // Loads all users from file into 'users' map
    void saveUsers();           // Saves all users from 'users' map into file

    std::string hashPassword(const std::string& password, const std::string& salt = ""); // Hash + salt password(extra random string)
    std::string generateRandomSalt(); // Creates a random salt string


public:
    Auth(const std::string& filename = "users.db"); // Constructor, default filename = users.db

    bool registerUser(const std::string& username, const std::string& password); // Add new user
    bool loginUser(const std::string& username, const std::string& password);    // Check username + password

    // Extra utilities
    bool userExists(const std::string& username); // Check if username is already registered
    bool changePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
    void clearAllUsers(); // Delete all accounts (for testing)
};

#endif

