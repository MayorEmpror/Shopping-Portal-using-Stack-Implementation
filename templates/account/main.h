#ifndef ACCOUNT_MAIN_H
#define ACCOUNT_MAIN_H

#include <iostream>
#include <string>
using namespace std;

class Account {
private:
    double totalbalance;
    double lifetimespent;
    string paymentmethods;

public:
    Account(double totalbalance, double lifetimespent, string paymentmethods)
        : totalbalance(totalbalance),
          lifetimespent(lifetimespent),
          paymentmethods(paymentmethods) {}

    Account(const Account& other) {
        this->lifetimespent  = other.lifetimespent;
        this->paymentmethods = other.paymentmethods;
        this->totalbalance   = other.totalbalance;
    }

    Account operator=(const Account& other) {
        if (this != &other) {
            this->lifetimespent  = other.lifetimespent;
            this->paymentmethods = other.paymentmethods;
            this->totalbalance   = other.totalbalance;
        }
        return *this;
    }

    double gettotalbalence()       { return this->totalbalance; }
    double getlifetimespent()      { return this->lifetimespent; }
    string getpaymentmethods()     { return this->paymentmethods; }

    void settotalbalence(double newbal)              { this->totalbalance   = newbal; }
    void setlifetimespent(double newlifetimespent)   { this->lifetimespent  = newlifetimespent; }
    void setpayement(const string& newpaymentmethod) { this->paymentmethods = newpaymentmethod; }

    ~Account() {}
};

#endif