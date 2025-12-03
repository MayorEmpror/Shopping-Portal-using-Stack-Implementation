#pragma once

#include <iostream>
#include <string>
#include "../Cart/main.h"
#include "../account/main.h"

using namespace std;

// ---------------------------------------
// User class: profile + cart + account
// ---------------------------------------
class User {
private:
    string  username;
    int     password;      // simple demo password (int)
    string  type;          // e.g. "customer", "admin"
    CART    shoppingcart;
    Account useraccount;

public:
    // Default constructor (needed for global currentUser)
    User()
        : username(""),
          password(0),
          type("customer"),
          shoppingcart(),
          useraccount(0.0, 0.0, "none") {}

    // Parameterized constructor (create account)
    User(const string& username,
         int password,
         const string& type,
         double startingBalance,
         const string& paymentMethod)
        : username(username),
          password(password),
          type(type),
          shoppingcart(),
          useraccount(startingBalance, 0.0, paymentMethod) {}

    // Getters
    const string& getUsername()      const { return username; }
    const string& getType()          const { return type; }
    double        getBalance()             { return useraccount.gettotalbalence(); }
    double        getLifetimeSpent()       { return useraccount.getlifetimespent(); }

    CART&        getShoppingCart()        { return shoppingcart; }
    const CART&  getShoppingCart() const  { return shoppingcart; }

    // Setters / updates
    void setUsername(const string& newname) { username = newname; }
    void setType(const string& newtype)     { type     = newtype; }
    void changePassword(int newpass)        { password = newpass; }

    // Simple password check
    bool checkPassword(int entered) const {
        return entered == password;
    }

    // Cart operations
    void addToCart(Products& newProduct) {
        shoppingcart.addToCart(newProduct);
        cout << "Added to shopping cart for user: " << username << endl;
    }

    void removeFromCart(Products& toRemProd) {
        shoppingcart.removeFromCart(toRemProd);
    }

    // ---------------------------------------
    // Checkout: deduct from user's account
    // ---------------------------------------
    bool checkout() {
        double total = shoppingcart.getTotalPrice();      // cart total
        double funds = useraccount.gettotalbalence();     // user balance

        if (total == 0.0) {
            cout << "Cart is empty. Nothing to buy." << endl;
            return false;
        }

        if (funds < total) {
            cout << "Transaction unsuccessful, insufficient funds." << endl;
            return false;
        }

        // Deduct and update lifetime spent
        useraccount.settotalbalence(funds - total);
        useraccount.setlifetimespent(useraccount.getlifetimespent() + total);

        cout << "Transaction successful, your delivery is on its way!" << endl;
        return true;
    }
};