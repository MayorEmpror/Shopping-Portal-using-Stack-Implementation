// main/UserStorage.hpp
#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "../templates/userProfile/main.h"

class UserStorage {
public:
    static const std::string USER_FILE;

    // Save user to file (overwrites existing user with same username)
    static bool saveUser(const User& user) {
        std::map<std::string, User> users = loadAllUsers();
        
        // Update or add this user
        users[user.getUsername()] = user;
        
        // Write all users back to file
        std::ofstream file(USER_FILE);
        if (!file.is_open()) {
            return false;
        }
        
        for (const auto& pair : users) {
            const User& u = pair.second;
            file << u.getUsername() << "|"
                 << u.getPassword() << "|"
                 << u.getType() << "|"
                 << const_cast<User&>(u).getBalance() << "|"
                 << const_cast<User&>(u).getLifetimeSpent() << "|"
                 << const_cast<User&>(u).getPaymentMethod() << "\n";
        }
        
        file.close();
        return true;
    }
    
    // Load user by username (returns empty User if not found)
    static User loadUser(const std::string& username) {
        std::map<std::string, User> users = loadAllUsers();
        auto it = users.find(username);
        if (it != users.end()) {
            return it->second;
        }
        return User(); // empty user
    }
    
    // Load all users from file
    static std::map<std::string, User> loadAllUsers() {
        std::map<std::string, User> users;
        std::ifstream file(USER_FILE);
        if (!file.is_open()) {
            return users; // empty map
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string username, passwordStr, type, balanceStr, lifetimeStr, paymentMethod;
            
            if (std::getline(iss, username, '|') &&
                std::getline(iss, passwordStr, '|') &&
                std::getline(iss, type, '|') &&
                std::getline(iss, balanceStr, '|') &&
                std::getline(iss, lifetimeStr, '|') &&
                std::getline(iss, paymentMethod, '|')) {
                
                try {
                    int password = std::stoi(passwordStr);
                    double balance = std::stod(balanceStr);
                    double lifetime = std::stod(lifetimeStr);
                    
                    User u(username, password, type, balance, paymentMethod);
                    // Set lifetime spent separately
                    u.setLifetimeSpent(lifetime);
                    users[username] = u;
                } catch (...) {
                    // Skip malformed lines
                    continue;
                }
            }
        }
        
        file.close();
        return users;
    }
    
    // Check if username exists
    static bool userExists(const std::string& username) {
        std::map<std::string, User> users = loadAllUsers();
        return users.find(username) != users.end();
    }
    
    // Delete user by username
    static bool deleteUser(const std::string& username) {
        std::map<std::string, User> users = loadAllUsers();
        
        // Remove the user
        auto it = users.find(username);
        if (it == users.end()) {
            return false; // User not found
        }
        
        users.erase(it);
        
        // Write remaining users back to file
        std::ofstream file(USER_FILE);
        if (!file.is_open()) {
            return false;
        }
        
        for (const auto& pair : users) {
            const User& u = pair.second;
            file << u.getUsername() << "|"
                 << u.getPassword() << "|"
                 << u.getType() << "|"
                 << const_cast<User&>(u).getBalance() << "|"
                 << const_cast<User&>(u).getLifetimeSpent() << "|"
                 << const_cast<User&>(u).getPaymentMethod() << "\n";
        }
        
        file.close();
        return true;
    }
};

const std::string UserStorage::USER_FILE = "users.txt";