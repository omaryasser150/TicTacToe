#include <gtest/gtest.h>       // Google Test framework
#include "Auth.h"              // Auth Header
#include <filesystem>          // Used for removing the test database file

//Runs before/after every test
class AuthTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up any existing test files
        std::filesystem::remove("test_users.db");
    }
    
    void TearDown() override {
        // Clean up test files
        std::filesystem::remove("test_users.db");
    }
};

TEST_F(AuthTest, UserRegistration) {
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Test successful registration
    EXPECT_TRUE(auth.registerUser("testuser", "password123"));
    EXPECT_TRUE(auth.userExists("testuser"));
    
    // Test duplicate registration fails
    EXPECT_FALSE(auth.registerUser("testuser", "different_password"));
    
    // Test empty username/password fails
    EXPECT_FALSE(auth.registerUser("", "password"));
    EXPECT_FALSE(auth.registerUser("username", ""));
    EXPECT_FALSE(auth.registerUser("", ""));
    
    // Test username with invalid characters fails
    EXPECT_FALSE(auth.registerUser("user:name", "password")); //rejects usernames containing : Because users are stored like this in a file: username:salt:hashedPassword

}

TEST_F(AuthTest, UserLogin) {
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Register a user first
    ASSERT_TRUE(auth.registerUser("logintest", "mypassword"));
    
    // Test successful login
    EXPECT_TRUE(auth.loginUser("logintest", "mypassword"));
    
    // Test wrong password
    EXPECT_FALSE(auth.loginUser("logintest", "wrongpassword"));
    
    // Test non-existent user
    EXPECT_FALSE(auth.loginUser("nonexistent", "password"));
    
    // Test empty credentials
    EXPECT_FALSE(auth.loginUser("", "password"));
    EXPECT_FALSE(auth.loginUser("logintest", ""));
}


TEST_F(AuthTest, MultipleUsers) {
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Register multiple users
    EXPECT_TRUE(auth.registerUser("user1", "pass1"));
    EXPECT_TRUE(auth.registerUser("user2", "pass2"));
    EXPECT_TRUE(auth.registerUser("user3", "pass3"));
    
    // Check all users exist
    EXPECT_TRUE(auth.userExists("user1"));
    EXPECT_TRUE(auth.userExists("user2"));
    EXPECT_TRUE(auth.userExists("user3"));
    EXPECT_FALSE(auth.userExists("user4"));
    
    // Test login for all users
    EXPECT_TRUE(auth.loginUser("user1", "pass1"));
    EXPECT_TRUE(auth.loginUser("user2", "pass2"));
    EXPECT_TRUE(auth.loginUser("user3", "pass3"));
    
    // Test cross-user password validation
    EXPECT_FALSE(auth.loginUser("user1", "pass2"));
    EXPECT_FALSE(auth.loginUser("user2", "pass3"));
    EXPECT_FALSE(auth.loginUser("user3", "pass1"));
}

//This test verifies that user data is saved to disk and is still accessible after restarting the application
TEST_F(AuthTest, Persistence) {
    // Create auth instance and register user
    {
        Auth auth("test_users.db");
        auth.clearAllUsers();
        ASSERT_TRUE(auth.registerUser("persistent", "password"));
    }
    
    // Create new auth instance (simulating app restart)
    {
        Auth auth("test_users.db");
        EXPECT_TRUE(auth.userExists("persistent"));
        EXPECT_TRUE(auth.loginUser("persistent", "password"));
    }
}

TEST_F(AuthTest, SpecialCharacters) {
    Auth auth("test_users.db");
    auth.clearAllUsers();
    
    // Test usernames and passwords with special characters
    EXPECT_TRUE(auth.registerUser("user@domain.com", "p@ssw0rd!"));
    EXPECT_TRUE(auth.loginUser("user@domain.com", "p@ssw0rd!"));
    
    EXPECT_TRUE(auth.registerUser("user123", "password with spaces"));
    EXPECT_TRUE(auth.loginUser("user123", "password with spaces"));
    
    // Test very long password
    std::string longPassword(100, 'a'); //100 a’s
    EXPECT_TRUE(auth.registerUser("longpass", longPassword));
    EXPECT_TRUE(auth.loginUser("longpass", longPassword));
}
