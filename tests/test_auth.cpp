// test_auth.cpp
#include "Auth.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>
#include <string>
#include <stdexcept>


#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }
#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Assertion failed: " #condition " should be false"); \
    }

void testAuthRegistration() {
    // Clean up any existing test files
    std::filesystem::remove("test_users.db");
    
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Test successful registration
    ASSERT_TRUE(auth.registerUser("testuser", "password123"));
    ASSERT_TRUE(auth.userExists("testuser"));
    
    // Test duplicate registration fails
    ASSERT_FALSE(auth.registerUser("testuser", "different_password"));
    
    // Test empty username/password fails
    ASSERT_FALSE(auth.registerUser("", "password"));
    ASSERT_FALSE(auth.registerUser("username", ""));
    ASSERT_FALSE(auth.registerUser("", ""));
    
    // Test username with invalid characters fails
    ASSERT_FALSE(auth.registerUser("user:name", "password"));
    
    // Clean up
    std::filesystem::remove("test_users.db");
}

void testAuthLogin() {
    std::filesystem::remove("test_users.db");
    
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Register a user first
    ASSERT_TRUE(auth.registerUser("logintest", "mypassword"));
    
    // Test successful login
    ASSERT_TRUE(auth.loginUser("logintest", "mypassword"));
    
    // Test wrong password
    ASSERT_FALSE(auth.loginUser("logintest", "wrongpassword"));
    
    // Test non-existent user
    ASSERT_FALSE(auth.loginUser("nonexistent", "password"));
    
    // Test empty credentials
    ASSERT_FALSE(auth.loginUser("", "password"));
    ASSERT_FALSE(auth.loginUser("logintest", ""));
    
    std::filesystem::remove("test_users.db");
}

void testAuthPasswordChange() {
    std::filesystem::remove("test_users.db");
    
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Register a user
    ASSERT_TRUE(auth.registerUser("changetest", "oldpassword"));
    
    // Test successful password change
    ASSERT_TRUE(auth.changePassword("changetest", "oldpassword", "newpassword"));
    
    // Verify old password no longer works
    ASSERT_FALSE(auth.loginUser("changetest", "oldpassword"));
    
    // Verify new password works
    ASSERT_TRUE(auth.loginUser("changetest", "newpassword"));
    
    // Test password change with wrong old password
    ASSERT_FALSE(auth.changePassword("changetest", "wrongold", "anothernew"));
    
    // Test password change for non-existent user
    ASSERT_FALSE(auth.changePassword("nonexistent", "old", "new"));
    
    std::filesystem::remove("test_users.db");
}

void testAuthMultipleUsers() {
    std::filesystem::remove("test_users.db");
    
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Register multiple users
    ASSERT_TRUE(auth.registerUser("user1", "pass1"));
    ASSERT_TRUE(auth.registerUser("user2", "pass2"));
    ASSERT_TRUE(auth.registerUser("user3", "pass3"));
    
    // Check all users exist
    ASSERT_TRUE(auth.userExists("user1"));
    ASSERT_TRUE(auth.userExists("user2"));
    ASSERT_TRUE(auth.userExists("user3"));
    ASSERT_FALSE(auth.userExists("user4"));
    
    // Test login for all users
    ASSERT_TRUE(auth.loginUser("user1", "pass1"));
    ASSERT_TRUE(auth.loginUser("user2", "pass2"));
    ASSERT_TRUE(auth.loginUser("user3", "pass3"));
    
    // Test cross-user password validation
    ASSERT_FALSE(auth.loginUser("user1", "pass2"));
    ASSERT_FALSE(auth.loginUser("user2", "pass3"));
    ASSERT_FALSE(auth.loginUser("user3", "pass1"));
    
    std::filesystem::remove("test_users.db");
}

void testAuthPersistence() {
    std::filesystem::remove("test_users.db");
    
    // Create auth instance and register user
    {
        Auth auth("test_users.db");
        auth.clearAllUsers();
        ASSERT_TRUE(auth.registerUser("persistent", "password"));
    }
    
    // Create new auth instance (simulating app restart)
    {
        Auth auth("test_users.db");
        ASSERT_TRUE(auth.userExists("persistent"));
        ASSERT_TRUE(auth.loginUser("persistent", "password"));
    }
    
    std::filesystem::remove("test_users.db");
}

void testAuthSpecialCharacters() {
    std::filesystem::remove("test_users.db");
    
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Test usernames and passwords with special characters
    ASSERT_TRUE(auth.registerUser("user@domain.com", "p@ssw0rd!"));
    ASSERT_TRUE(auth.loginUser("user@domain.com", "p@ssw0rd!"));
    
    ASSERT_TRUE(auth.registerUser("user123", "password with spaces"));
    ASSERT_TRUE(auth.loginUser("user123", "password with spaces"));
    
    // Test very long password
    std::string longPassword(100, 'a');
    ASSERT_TRUE(auth.registerUser("longpass", longPassword));
    ASSERT_TRUE(auth.loginUser("longpass", longPassword));
    
    std::filesystem::remove("test_users.db");
}

void testAuthClearUsers() {
    std::filesystem::remove("test_users.db");
    
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Register some users
    ASSERT_TRUE(auth.registerUser("user1", "pass1"));
    ASSERT_TRUE(auth.registerUser("user2", "pass2"));
    ASSERT_TRUE(auth.userExists("user1"));
    ASSERT_TRUE(auth.userExists("user2"));
    
    // Clear all users
    auth.clearAllUsers();
    
    // Verify users are gone
    ASSERT_FALSE(auth.userExists("user1"));
    ASSERT_FALSE(auth.userExists("user2"));
    
    std::filesystem::remove("test_users.db");
}

void runAuthTests(TestRunner& testRunner) {
    testRunner.addTest("Auth Registration", testAuthRegistration);
    testRunner.addTest("Auth Login", testAuthLogin);
    testRunner.addTest("Auth Password Change", testAuthPasswordChange);
    testRunner.addTest("Auth Multiple Users", testAuthMultipleUsers);
    testRunner.addTest("Auth Persistence", testAuthPersistence);
    testRunner.addTest("Auth Special Characters", testAuthSpecialCharacters);
    testRunner.addTest("Auth Clear Users", testAuthClearUsers);
}