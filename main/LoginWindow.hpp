// main/LoginWindow.hpp
#pragma once

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

#include "../templates/userProfile/main.h"   // User

class LoginWindow : public QDialog {
public:
    explicit LoginWindow(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Login / Sign Up");
        setFixedSize(350, 260);

        // ---- TOP‑LEVEL LAYOUT ----
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QTabWidget *tabs = new QTabWidget(this);

        // =====================================================
        // LOGIN TAB
        // =====================================================
        QWidget *loginTab = new QWidget(this);
        QVBoxLayout *loginLayout = new QVBoxLayout(loginTab);

        QFormLayout *loginForm = new QFormLayout();   // no parent here

        loginUsername = new QLineEdit();
        loginPassword = new QLineEdit();
        loginPassword->setEchoMode(QLineEdit::Password);

        loginForm->addRow("Username:", loginUsername);
        loginForm->addRow("Password:", loginPassword);

        QPushButton *loginBtn = new QPushButton("Login");

        loginLayout->addLayout(loginForm);
        loginLayout->addWidget(loginBtn);
        loginLayout->addStretch();

        // =====================================================
        // SIGNUP TAB
        // =====================================================
        QWidget *signupTab = new QWidget(this);
        QVBoxLayout *signupLayout = new QVBoxLayout(signupTab);

        QFormLayout *signupForm = new QFormLayout();  // no parent here

        signupUsername = new QLineEdit();
        signupPassword = new QLineEdit();
        signupPassword->setEchoMode(QLineEdit::Password);
        signupType = new QLineEdit();           // e.g. "customer"
        signupBalance = new QLineEdit();        // starting balance
        signupPayment = new QLineEdit();        // payment method string

        signupForm->addRow("Username:", signupUsername);
        signupForm->addRow("Password:", signupPassword);
        signupForm->addRow("Type:", signupType);
        signupForm->addRow("Start Balance:", signupBalance);
        signupForm->addRow("Payment Method:", signupPayment);

        QPushButton *signupBtn = new QPushButton("Sign Up");

        signupLayout->addLayout(signupForm);
        signupLayout->addWidget(signupBtn);
        signupLayout->addStretch();

        // =====================================================
        // TABS + MAIN LAYOUT
        // =====================================================
        tabs->addTab(loginTab, "Login");
        tabs->addTab(signupTab, "Sign Up");

        mainLayout->addWidget(tabs);

        // =====================================================
        // CONNECTIONS (INLINE LAMBDAS)
        // =====================================================

        // LOGIN
        QObject::connect(loginBtn, &QPushButton::clicked, this, [this]() {
            QMessageBox::information(this, "Debug", "Login clicked");  // MUST appear

            QString name    = loginUsername->text();
            QString passStr = loginPassword->text();

            if (name.isEmpty() || passStr.isEmpty()) {
                QMessageBox::warning(this, "Error", "Please enter username and password.");
                return;
            }

            currentUser = User(name.toStdString(),
                               0,         // ignore real password for now
                               "customer",
                               500.0,
                               "card");

            accept();   // closes dialog with Accepted
        });

        // SIGN UP
        QObject::connect(signupBtn, &QPushButton::clicked, this, [this]() {
            QMessageBox::information(this, "Debug", "Sign Up clicked");  // MUST appear

            QString name    = signupUsername->text();
            QString passStr = signupPassword->text();
            QString typeStr = signupType->text();
            QString balStr  = signupBalance->text();
            QString payStr  = signupPayment->text();

            if (name.isEmpty() || passStr.isEmpty() || typeStr.isEmpty()
                || balStr.isEmpty() || payStr.isEmpty()) {
                QMessageBox::warning(this, "Error", "Please fill in all fields.");
                return;
            }

            bool okBal = false;
            double balance = balStr.toDouble(&okBal);
            if (!okBal) balance = 500.0;

            currentUser = User(name.toStdString(),
                               0,
                               typeStr.toStdString(),
                               balance,
                               payStr.toStdString());

            accept();   // closes dialog with Accepted
        });
    }

    User getCurrentUser() const {
        return currentUser;
    }

private:
    // login widgets
    QLineEdit *loginUsername{};
    QLineEdit *loginPassword{};

    // signup widgets
    QLineEdit *signupUsername{};
    QLineEdit *signupPassword{};
    QLineEdit *signupType{};
    QLineEdit *signupBalance{};
    QLineEdit *signupPayment{};

    User currentUser;
};