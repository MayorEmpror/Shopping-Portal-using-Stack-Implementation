#include <QApplication>
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QPixmap>
#include <QStackedWidget>
#include <QStyleOption>
#include <QPainter>
#include <vector>
#include <QMessageBox>
#include <QSpacerItem>
#include <stack>
#include <QCheckBox>
#include <QLineEdit>

#include "../templates/products/main.h"
#include "../data/DataArray.h"
#include "./LoginWindow.hpp"
#include "./UserStorage.hpp"
#include "../templates/userProfile/main.h"
#include "../templates/coupons/main.h"

// Global logged‑in user
User currentUser;
QLabel *gUserLabel   = nullptr;
QLabel *gBalanceLabel = nullptr;

// Global predefined coupons
std::vector<Coupons> availableCoupons = {
    Coupons("SAVE10", "Get 10% off your purchase", 10),
    Coupons("SAVE20", "Get 20% off your purchase", 20),
    Coupons("HALF50", "Get 50% off your purchase", 50),
    Coupons("WELCOME15", "Welcome discount - 15% off", 15),
    Coupons("MEGA30", "Mega sale - 30% off", 30)
};

// ----------------------------------------------
// DarkWidget (for background styling, if needed)
// ----------------------------------------------
class DarkWidget : public QWidget {
public:
    using QWidget::QWidget;
protected:
    void paintEvent(QPaintEvent *) override {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

// ----------------------------------------------
// Create Product Card
// ----------------------------------------------
QFrame* createProductCard(const Products& product) {
    QFrame *card = new QFrame();
    card->setFixedSize(220, 340);
    card->setStyleSheet(R"(
        QFrame {
            background-color: #1c1c1c;
            border-radius: 12px;
            padding: 10px;
        }
        QFrame:hover {
            background-color: #2a2a2a;
        }
        QLabel {
            color: #ffffff;
        }
        QPushButton {
            background-color: #007bff;
            border-radius: 8px;
            padding: 6px;
            color: white;
        }
        QPushButton:hover {
            background-color: #2690ff;
        }
    )");

    QVBoxLayout *vbox = new QVBoxLayout(card);
    vbox->setAlignment(Qt::AlignTop);

    // Image
    QLabel *img = new QLabel();
    img->setFixedSize(150, 150);
    img->setAlignment(Qt::AlignCenter);

    QPixmap pix(QString::fromStdString(product.getMainImage()));
    if (!pix.isNull()) {
        img->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        img->setStyleSheet("background-color: #333333; border-radius: 10px;");
        img->setText("🖼");
    }

    // Name
    QLabel *name = new QLabel(QString::fromStdString(product.getname()));
    name->setWordWrap(true);
    name->setStyleSheet("font-size: 14px;");

    // Price
    QLabel *price = new QLabel("$" + QString::number(product.getprice(), 'f', 2));
    price->setStyleSheet("font-weight:bold; color:#00ff00;");

    // Features
    QLabel *features = new QLabel(QString::fromStdString(product.getfeatures()));
    features->setWordWrap(true);
    features->setStyleSheet("font-size: 11px; color:#cccccc;");

    // Buy Button
    QPushButton *buyBtn = new QPushButton("Buy");
    QObject::connect(buyBtn, &QPushButton::clicked, [product]() {
        // add to the current user's cart
        Products &nonConst = const_cast<Products&>(product);
        currentUser.addToCart(nonConst);
        // Optionally show confirmation
        // QMessageBox::information(nullptr, "Cart",
        //     QString::fromStdString(product.getname()) + " added to cart.");
    });

    // Layout
    vbox->addWidget(img, 0, Qt::AlignCenter);
    vbox->addSpacing(8);
    vbox->addWidget(name, 0, Qt::AlignCenter);
    vbox->addWidget(price, 0, Qt::AlignCenter);
    vbox->addWidget(features, 0, Qt::AlignCenter);
    vbox->addStretch(1);
    vbox->addWidget(buyBtn, 0, Qt::AlignCenter);

    return card;
}

// ----------------------------------------------
// Build Catalog Page
// ----------------------------------------------
QWidget* buildCatalogPage() {
    QWidget *catalog = new QWidget();
    QGridLayout *grid = new QGridLayout(catalog);
    grid->setSpacing(20);
    grid->setContentsMargins(20, 20, 20, 20);

    std::vector<Products> productList = InitDBFrame();

    for (size_t i = 0; i < productList.size(); ++i) {
        QFrame *card = createProductCard(productList[i]);
        grid->addWidget(card, static_cast<int>(i / 3), static_cast<int>(i % 3));
    }

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(catalog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    QWidget *wrapper = new QWidget();
    QVBoxLayout *v = new QVBoxLayout(wrapper);
    v->addWidget(scrollArea);
    v->setContentsMargins(0, 0, 0, 0);

    return wrapper;
}

// ----------------------------------------------
// Build Cart Page
// ----------------------------------------------
QWidget* buildCartPage(QStackedWidget *stackedWidget = nullptr) {
    QWidget *cartPage = new QWidget();
    cartPage->setStyleSheet("background-color: #0a0a0a;");

    QHBoxLayout *mainLayout = new QHBoxLayout(cartPage);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(30);

    // ========== LEFT SIDE: Cart Items ==========
    QWidget *itemsContainer = new QWidget();
    itemsContainer->setStyleSheet("background-color: transparent;");
    QVBoxLayout *itemsLayout = new QVBoxLayout(itemsContainer);
    itemsLayout->setContentsMargins(0, 0, 0, 0);
    itemsLayout->setSpacing(20);

    // Title
    QLabel *title = new QLabel("Your Cart");
    title->setStyleSheet("color: #ffffff; font-size: 28px; font-weight: bold;");
    itemsLayout->addWidget(title);

    // Scroll area for items
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet(R"(
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        QScrollBar:vertical {
            background-color: #1a1a1a;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background-color: #333333;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #444444;
        }
    )");

    QWidget *container = new QWidget();
    QVBoxLayout *listLayout = new QVBoxLayout(container);
    listLayout->setSpacing(15);
    listLayout->setContentsMargins(0, 0, 0, 0);

    CART &cart = currentUser.getShoppingCart();
    std::stack<Products> s = cart.getItemsToBuy();
    std::vector<Products> productsList; // Store products for removal

    // Convert stack to vector (for easier iteration and removal)
    while (!s.empty()) {
        productsList.push_back(s.top());
        s.pop();
    }

    // Create product cards
    for (size_t i = 0; i < productsList.size(); ++i) {
        Products p = productsList[i];
        
        QFrame *itemFrame = new QFrame();
        itemFrame->setStyleSheet(R"(
            QFrame {
                background-color: #1a1a1a;
                border: 1px solid #2a2a2a;
                border-radius: 12px;
                padding: 0px;
            }
            QFrame:hover {
                border-color: #333333;
            }
        )");
        itemFrame->setMinimumHeight(120);

        QHBoxLayout *h = new QHBoxLayout(itemFrame);
        h->setContentsMargins(15, 15, 15, 15);
        h->setSpacing(15);

        // Checkbox (optional - for future selection feature)
        QCheckBox *checkbox = new QCheckBox();
        checkbox->setChecked(true);
        checkbox->setStyleSheet(R"(
            QCheckBox {
                spacing: 8px;
            }
            QCheckBox::indicator {
                width: 20px;
                height: 20px;
                border: 2px solid #444444;
                border-radius: 4px;
                background-color: #1a1a1a;
            }
            QCheckBox::indicator:checked {
                background-color: #4a9eff;
                border-color: #4a9eff;
            }
            QCheckBox::indicator:hover {
                border-color: #4a9eff;
            }
        )");
        h->addWidget(checkbox);

        // Product Image
        QLabel *img = new QLabel();
        img->setFixedSize(90, 90);
        img->setStyleSheet(R"(
            QLabel {
                background-color: #0a0a0a;
                border-radius: 8px;
                border: 1px solid #2a2a2a;
            }
        )");
        QPixmap pix(QString::fromStdString(p.getMainImage()));
        if (!pix.isNull()) {
            img->setPixmap(pix.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            img->setText("🖼");
            img->setAlignment(Qt::AlignCenter);
        }
        h->addWidget(img);

        // Product Info (Name, Features, Price)
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(6);
        infoLayout->setContentsMargins(0, 0, 0, 0);

        QLabel *name = new QLabel(QString::fromStdString(p.getname()));
        name->setStyleSheet("color: #ffffff; font-size: 16px; font-weight: 500;");
        name->setWordWrap(true);
        infoLayout->addWidget(name);

        QLabel *features = new QLabel(QString::fromStdString(p.getfeatures()));
        features->setStyleSheet("color: #888888; font-size: 12px;");
        features->setWordWrap(true);
        infoLayout->addWidget(features);

        QLabel *price = new QLabel("$" + QString::number(p.getprice(), 'f', 2));
        price->setStyleSheet("color: #00ff88; font-size: 18px; font-weight: bold; margin-top: 8px;");
        infoLayout->addWidget(price);

        infoLayout->addStretch();
        h->addLayout(infoLayout, 1);

        // Remove button
        QPushButton *removeBtn = new QPushButton("✕");
        removeBtn->setFixedSize(32, 32);
        removeBtn->setStyleSheet(R"(
            QPushButton {
                background-color: transparent;
                border: 1px solid #333333;
                border-radius: 6px;
                color: #888888;
                font-size: 18px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #2a2a2a;
                border-color: #ff4444;
                color: #ff4444;
            }
            QPushButton:pressed {
                background-color: #1a1a1a;
            }
        )");

        // Store product for removal
        Products productToRemove = p;
        QObject::connect(removeBtn, &QPushButton::clicked, [productToRemove, stackedWidget]() {
            currentUser.removeFromCart(const_cast<Products&>(productToRemove));
            UserStorage::saveUser(currentUser);
            
            // Refresh the cart page by rebuilding it
            if (stackedWidget) {
                QWidget *oldCart = stackedWidget->widget(1);
                if (oldCart) {
                    stackedWidget->removeWidget(oldCart);
                    delete oldCart;
                }
                QWidget *newCartPage = buildCartPage(stackedWidget);
                stackedWidget->insertWidget(1, newCartPage);
                stackedWidget->setCurrentIndex(1);
            }
        });

        h->addWidget(removeBtn);

        listLayout->addWidget(itemFrame);
    }

    if (productsList.empty()) {
        QLabel *emptyLabel = new QLabel("Your cart is empty");
        emptyLabel->setStyleSheet("color: #666666; font-size: 16px; padding: 40px;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        listLayout->addWidget(emptyLabel);
    }

    listLayout->addStretch(1);
    scroll->setWidget(container);
    itemsLayout->addWidget(scroll, 1);

    // ========== RIGHT SIDE: Summary ==========
    QWidget *summaryContainer = new QWidget();
    summaryContainer->setFixedWidth(320);
    summaryContainer->setStyleSheet("background-color: transparent;");
    QVBoxLayout *summaryLayout = new QVBoxLayout(summaryContainer);
    summaryLayout->setContentsMargins(0, 0, 0, 0);
    summaryLayout->setSpacing(20);

    // Price Details Card
    QFrame *summaryCard = new QFrame();
    summaryCard->setStyleSheet(R"(
        QFrame {
            background-color: #1a1a1a;
            border: 1px solid #2a2a2a;
            border-radius: 12px;
            padding: 20px;
        }
    )");
    QVBoxLayout *cardLayout = new QVBoxLayout(summaryCard);
    cardLayout->setSpacing(15);
    cardLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *priceTitle = new QLabel("Price Details");
    priceTitle->setStyleSheet("color: #ffffff; font-size: 18px; font-weight: bold;");
    cardLayout->addWidget(priceTitle);

    // Calculate totals
    double subtotal = 0;
    std::stack<Products> temp = cart.getItemsToBuy();
    while (!temp.empty()) {
        subtotal += temp.top().getprice();
        temp.pop();
    }

    // Items count
    QHBoxLayout *itemsRow = new QHBoxLayout();
    QLabel *itemsLabel = new QLabel(QString("%1 item%2").arg(productsList.size()).arg(productsList.size() != 1 ? "s" : ""));
    itemsLabel->setStyleSheet("color: #888888; font-size: 14px;");
    QLabel *itemsPrice = new QLabel("$" + QString::number(subtotal, 'f', 2));
    itemsPrice->setStyleSheet("color: #ffffff; font-size: 14px;");
    itemsRow->addWidget(itemsLabel);
    itemsRow->addStretch();
    itemsRow->addWidget(itemsPrice);
    cardLayout->addLayout(itemsRow);

    // Delivery (always free for now)
    QHBoxLayout *deliveryRow = new QHBoxLayout();
    QLabel *deliveryLabel = new QLabel("Delivery Charges");
    deliveryLabel->setStyleSheet("color: #888888; font-size: 14px;");
    QLabel *deliveryPrice = new QLabel("Free Delivery");
    deliveryPrice->setStyleSheet("color: #00ff88; font-size: 14px; font-weight: bold;");
    deliveryRow->addWidget(deliveryLabel);
    deliveryRow->addStretch();
    deliveryRow->addWidget(deliveryPrice);
    cardLayout->addLayout(deliveryRow);

    // Coupon Section
    cardLayout->addSpacing(10);
    
    QLabel *couponLabel = new QLabel("Have a coupon?");
    couponLabel->setStyleSheet("color: #888888; font-size: 13px; font-weight: bold;");
    cardLayout->addWidget(couponLabel);
    
    QHBoxLayout *couponInputRow = new QHBoxLayout();
    couponInputRow->setSpacing(8);
    
    QLineEdit *couponInput = new QLineEdit();
    couponInput->setPlaceholderText("Enter coupon code");
    couponInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #0a0a0a;
            border: 1px solid #444444;
            border-radius: 6px;
            padding: 8px;
            color: #ffffff;
            font-size: 13px;
        }
        QLineEdit:focus {
            border-color: #4a9eff;
        }
    )");
    couponInputRow->addWidget(couponInput);
    
    QPushButton *applyBtn = new QPushButton("Apply");
    applyBtn->setFixedWidth(70);
    applyBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a9eff;
            color: white;
            font-size: 13px;
            font-weight: bold;
            padding: 8px;
            border-radius: 6px;
            border: none;
        }
        QPushButton:hover {
            background-color: #3a8eef;
        }
        QPushButton:pressed {
            background-color: #2a7edf;
        }
    )");
    couponInputRow->addWidget(applyBtn);
    cardLayout->addLayout(couponInputRow);
    
    // Discount row (initially hidden)
    QHBoxLayout *discountRow = new QHBoxLayout();
    QLabel *discountLabel = new QLabel("Discount");
    discountLabel->setStyleSheet("color: #888888; font-size: 14px;");
    QLabel *discountAmount = new QLabel("- $0.00");
    discountAmount->setStyleSheet("color: #ff6b6b; font-size: 14px; font-weight: bold;");
    discountRow->addWidget(discountLabel);
    discountRow->addStretch();
    discountRow->addWidget(discountAmount);
    
    // Hide discount row initially
    discountLabel->setVisible(false);
    discountAmount->setVisible(false);
    cardLayout->addLayout(discountRow);

    // Separator line
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("QFrame { background-color: #2a2a2a; max-height: 1px; }");
    cardLayout->addWidget(separator);

    // Total Amount
    QHBoxLayout *totalRow = new QHBoxLayout();
    QLabel *totalLabel = new QLabel("Total Amount");
    totalLabel->setStyleSheet("color: #ffffff; font-size: 18px; font-weight: bold;");
    QLabel *totalPrice = new QLabel("$" + QString::number(subtotal, 'f', 2));
    totalPrice->setStyleSheet("color: #ffffff; font-size: 20px; font-weight: bold;");
    totalRow->addWidget(totalLabel);
    totalRow->addStretch();
    totalRow->addWidget(totalPrice);
    cardLayout->addLayout(totalRow);

    summaryLayout->addWidget(summaryCard);
    
    // Store discount in totalPrice as a property
    totalPrice->setProperty("appliedDiscount", 0.0);
    totalPrice->setProperty("originalSubtotal", subtotal);
    
    // Coupon apply logic
    QObject::connect(applyBtn, &QPushButton::clicked, [couponInput, applyBtn, discountLabel, discountAmount, totalPrice, subtotal]() {
        QString couponCode = couponInput->text().trimmed().toUpper();
        
        if (couponCode.isEmpty()) {
            QMessageBox::warning(nullptr, "Invalid Coupon", "Please enter a coupon code.");
            return;
        }
        
        // Check if coupon is valid
        bool validCoupon = false;
        int discountPercent = 0;
        
        for (const auto& coupon : availableCoupons) {
            if (QString::fromStdString(coupon.getname()).toUpper() == couponCode) {
                validCoupon = true;
                discountPercent = coupon.getdiscount();
                break;
            }
        }
        
        if (validCoupon) {
            // Calculate discount
            double currentDiscount = (subtotal * discountPercent) / 100.0;
            double finalTotal = subtotal - currentDiscount;
            
            // Store discount in property
            totalPrice->setProperty("appliedDiscount", currentDiscount);
            
            // Update UI
            discountLabel->setVisible(true);
            discountAmount->setVisible(true);
            discountAmount->setText("- $" + QString::number(currentDiscount, 'f', 2) + 
                                   QString(" (%1%)").arg(discountPercent));
            totalPrice->setText("$" + QString::number(finalTotal, 'f', 2));
            
            // Disable input and button
            couponInput->setEnabled(false);
            applyBtn->setEnabled(false);
            applyBtn->setText("Applied");
            applyBtn->setStyleSheet(R"(
                QPushButton {
                    background-color: #00ff88;
                    color: #0a0a0a;
                    font-size: 13px;
                    font-weight: bold;
                    padding: 8px;
                    border-radius: 6px;
                    border: none;
                }
            )");
            
            QMessageBox::information(nullptr, "Coupon Applied", 
                QString("Coupon '%1' applied successfully!\nYou saved $%2 (%3% off)")
                    .arg(couponCode)
                    .arg(currentDiscount, 0, 'f', 2)
                    .arg(discountPercent));
        } else {
            QMessageBox::warning(nullptr, "Invalid Coupon", 
                QString("Coupon code '%1' is not valid.\n\nAvailable coupons:\n"
                        "• SAVE10 (10% off)\n"
                        "• SAVE20 (20% off)\n"
                        "• HALF50 (50% off)\n"
                        "• WELCOME15 (15% off)\n"
                        "• MEGA30 (30% off)")
                    .arg(couponCode));
        }
    });

    // Checkout Button
    QPushButton *checkoutBtn = new QPushButton("Checkout");
    checkoutBtn->setMinimumHeight(52);
    checkoutBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a9eff;
            color: white;
            font-size: 16px;
            font-weight: bold;
            padding: 14px;
            border-radius: 8px;
            border: none;
        }
        QPushButton:hover {
            background-color: #3a8eef;
        }
        QPushButton:pressed {
            background-color: #2a7edf;
        }
    )");
    summaryLayout->addWidget(checkoutBtn);

    summaryLayout->addStretch();

    // Checkout logic with coupon discount support
    QObject::connect(checkoutBtn, &QPushButton::clicked, [totalPrice, stackedWidget]() {
        // Get the cart total (with any applied discount)
        double originalTotal = currentUser.getShoppingCart().getTotalPrice();
        double appliedDiscount = totalPrice->property("appliedDiscount").toDouble();
        double finalTotal = originalTotal - appliedDiscount;
        
        if (originalTotal == 0.0) {
            QMessageBox::warning(nullptr, "Empty Cart", "Your cart is empty. Nothing to checkout.");
            return;
        }
        
        double userBalance = currentUser.getBalance();
        
        if (userBalance < finalTotal) {
            QMessageBox::warning(nullptr, "Insufficient funds",
                QString("Insufficient funds in your account.\n\nTotal: $%1\nYour Balance: $%2")
                    .arg(finalTotal, 0, 'f', 2)
                    .arg(userBalance, 0, 'f', 2));
        } else {
            // Deduct from balance and update lifetime spent
            currentUser.setBalance(userBalance - finalTotal);
            currentUser.setLifetimeSpent(currentUser.getLifetimeSpent() + finalTotal);
            
            // Clear the cart
            CART &cart = currentUser.getShoppingCart();
            while (!cart.getItemsToBuy().empty()) {
                std::stack<Products> temp = cart.getItemsToBuy();
                Products p = temp.top();
                currentUser.removeFromCart(p);
            }
            
            // Save updated user data
            UserStorage::saveUser(currentUser);

            // Refresh balance label
            if (gBalanceLabel) {
                gBalanceLabel->setText(
                    QString("Balance: $%1").arg(currentUser.getBalance(), 0, 'f', 2)
                );
            }
            
            QString message = "Payment successful! Your order is on its way.";
            if (appliedDiscount > 0) {
                message += QString("\n\nYou saved $%1 with your coupon!").arg(appliedDiscount, 0, 'f', 2);
            }
            
            QMessageBox::information(nullptr, "Success", message);
            
            // Refresh cart page
            if (stackedWidget) {
                QWidget *oldCart = stackedWidget->widget(1);
                if (oldCart) {
                    stackedWidget->removeWidget(oldCart);
                    delete oldCart;
                }
                QWidget *newCartPage = buildCartPage(stackedWidget);
                stackedWidget->insertWidget(1, newCartPage);
                stackedWidget->setCurrentIndex(1);
            }
        }
    });

    // Add both sides to main layout
    mainLayout->addWidget(itemsContainer, 2);  // Items take 2/3 of space
    mainLayout->addWidget(summaryContainer, 1); // Summary takes 1/3

    return cartPage;
}

// ----------------------------------------------
// Build Admin Page
// ----------------------------------------------
QWidget* buildAdminPage(QStackedWidget *stackedWidget = nullptr) {
    QWidget *adminPage = new QWidget();
    adminPage->setStyleSheet("background-color: #0a0a0a;");

    QVBoxLayout *mainLayout = new QVBoxLayout(adminPage);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Title
    QLabel *title = new QLabel("Admin Panel - User Management");
    title->setStyleSheet("color: #ffffff; font-size: 28px; font-weight: bold;");
    mainLayout->addWidget(title);

    // Scroll area for users
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet(R"(
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        QScrollBar:vertical {
            background-color: #1a1a1a;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background-color: #333333;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #444444;
        }
    )");

    QWidget *container = new QWidget();
    QVBoxLayout *listLayout = new QVBoxLayout(container);
    listLayout->setSpacing(15);
    listLayout->setContentsMargins(0, 0, 0, 0);

    // Load all users
    std::map<std::string, User> allUsers = UserStorage::loadAllUsers();

    // Create header row
    QFrame *headerFrame = new QFrame();
    headerFrame->setStyleSheet(R"(
        QFrame {
            background-color: #1a1a1a;
            border: 1px solid #2a2a2a;
            border-radius: 8px;
            padding: 0px;
        }
    )");
    headerFrame->setMinimumHeight(50);

    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 10, 20, 10);
    headerLayout->setSpacing(15);

    QLabel *usernameHeader = new QLabel("Username");
    usernameHeader->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold;");
    usernameHeader->setFixedWidth(200);
    headerLayout->addWidget(usernameHeader);

    QLabel *typeHeader = new QLabel("Type");
    typeHeader->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold;");
    typeHeader->setFixedWidth(120);
    headerLayout->addWidget(typeHeader);

    QLabel *balanceHeader = new QLabel("Balance");
    balanceHeader->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold;");
    balanceHeader->setFixedWidth(150);
    headerLayout->addWidget(balanceHeader);

    QLabel *spentHeader = new QLabel("Lifetime Spent");
    spentHeader->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold;");
    spentHeader->setFixedWidth(150);
    headerLayout->addWidget(spentHeader);

    headerLayout->addStretch();

    QLabel *actionsHeader = new QLabel("Actions");
    actionsHeader->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold;");
    actionsHeader->setFixedWidth(100);
    headerLayout->addWidget(actionsHeader);

    listLayout->addWidget(headerFrame);

    // Create user rows
    for (const auto& pair : allUsers) {
        const User& user = pair.second;
        std::string username = user.getUsername();
        
        QFrame *userFrame = new QFrame();
        userFrame->setStyleSheet(R"(
            QFrame {
                background-color: #1a1a1a;
                border: 1px solid #2a2a2a;
                border-radius: 8px;
                padding: 0px;
            }
            QFrame:hover {
                border-color: #333333;
                background-color: #1f1f1f;
            }
        )");
        userFrame->setMinimumHeight(70);

        QHBoxLayout *h = new QHBoxLayout(userFrame);
        h->setContentsMargins(20, 15, 20, 15);
        h->setSpacing(15);

        // Username
        QLabel *usernameLabel = new QLabel(QString::fromStdString(username));
        usernameLabel->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: 500;");
        usernameLabel->setFixedWidth(200);
        h->addWidget(usernameLabel);

        // Type
        QLabel *typeLabel = new QLabel(QString::fromStdString(user.getType()));
        if (user.getType() == "admin") {
            typeLabel->setStyleSheet("color: #ff9900; font-size: 14px; font-weight: bold;");
        } else {
            typeLabel->setStyleSheet("color: #4a9eff; font-size: 14px;");
        }
        typeLabel->setFixedWidth(120);
        h->addWidget(typeLabel);

        // Balance
        QLabel *balanceLabel = new QLabel("$" + QString::number(const_cast<User&>(user).getBalance(), 'f', 2));
        balanceLabel->setStyleSheet("color: #00ff88; font-size: 14px; font-weight: 500;");
        balanceLabel->setFixedWidth(150);
        h->addWidget(balanceLabel);

        // Lifetime Spent
        QLabel *spentLabel = new QLabel("$" + QString::number(const_cast<User&>(user).getLifetimeSpent(), 'f', 2));
        spentLabel->setStyleSheet("color: #888888; font-size: 14px;");
        spentLabel->setFixedWidth(150);
        h->addWidget(spentLabel);

        h->addStretch();

        // Delete button
        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setFixedSize(80, 36);
        deleteBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #2a2a2a;
                border: 1px solid #444444;
                border-radius: 6px;
                color: #ff4444;
                font-size: 13px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #ff4444;
                border-color: #ff4444;
                color: #ffffff;
            }
            QPushButton:pressed {
                background-color: #dd2222;
            }
            QPushButton:disabled {
                background-color: #1a1a1a;
                border-color: #2a2a2a;
                color: #555555;
            }
        )");

        // Disable delete button if this is the current user
        if (username == currentUser.getUsername()) {
            deleteBtn->setEnabled(false);
            deleteBtn->setToolTip("Cannot delete your own account");
        }

        // Connect delete button
        QObject::connect(deleteBtn, &QPushButton::clicked, [username, stackedWidget]() {
            // Confirmation dialog
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Confirm Deletion",
                QString("Are you sure you want to delete user '%1'?\n\nThis action cannot be undone.")
                    .arg(QString::fromStdString(username)),
                QMessageBox::Yes | QMessageBox::No);
            
            if (reply == QMessageBox::Yes) {
                if (UserStorage::deleteUser(username)) {
                    QMessageBox::information(nullptr, "Success",
                        QString("User '%1' has been deleted successfully.")
                            .arg(QString::fromStdString(username)));
                    
                    // Refresh the admin page
                    if (stackedWidget) {
                        QWidget *oldAdmin = stackedWidget->widget(2);
                        if (oldAdmin) {
                            stackedWidget->removeWidget(oldAdmin);
                            delete oldAdmin;
                        }
                        QWidget *newAdminPage = buildAdminPage(stackedWidget);
                        stackedWidget->insertWidget(2, newAdminPage);
                        stackedWidget->setCurrentIndex(2);
                    }
                } else {
                    QMessageBox::warning(nullptr, "Error",
                        QString("Failed to delete user '%1'.")
                            .arg(QString::fromStdString(username)));
                }
            }
        });

        h->addWidget(deleteBtn);
        listLayout->addWidget(userFrame);
    }

    if (allUsers.empty()) {
        QLabel *emptyLabel = new QLabel("No users found");
        emptyLabel->setStyleSheet("color: #666666; font-size: 16px; padding: 40px;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        listLayout->addWidget(emptyLabel);
    }

    listLayout->addStretch(1);
    scroll->setWidget(container);
    mainLayout->addWidget(scroll, 1);

    // Info label at bottom
    QLabel *infoLabel = new QLabel(QString("Total Users: %1").arg(allUsers.size()));
    infoLabel->setStyleSheet("color: #888888; font-size: 14px; padding: 10px;");
    mainLayout->addWidget(infoLabel);

    return adminPage;
}

// ----------------------------------------------
// Main function
// ----------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Login window
    LoginWindow loginWindow;
    if (loginWindow.exec() != QDialog::Accepted) {
        return 1;
    }
    
    // Get the logged-in user
    currentUser = loginWindow.getCurrentUser();
    
    if (currentUser.getUsername().empty()) {
        QMessageBox::warning(nullptr, "Error", "Failed to load user.");
        return 1;
    }

    // Main window
    QWidget *mainWindow = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWindow);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Sidebar (menu + user info)
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    sidebarLayout->setContentsMargins(20, 20, 20, 20);
    sidebarLayout->setSpacing(20);

    QLabel *menuLabel = new QLabel("Menu");
    menuLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold;");
    sidebarLayout->addWidget(menuLabel);

    QPushButton *catalogBtn = new QPushButton("Catalog");
    catalogBtn->setStyleSheet("background-color: #292929; color: white; padding: 10px; border-radius: 8px;");
    sidebarLayout->addWidget(catalogBtn);

    QPushButton *cartBtn = new QPushButton("Cart");
    cartBtn->setStyleSheet("background-color: #292929; color: white; padding: 10px; border-radius: 8px;");
    sidebarLayout->addWidget(cartBtn);

    // Admin Panel button (only visible for admin users)
    QPushButton *adminBtn = nullptr;
    if (currentUser.getType() == "admin") {
        adminBtn = new QPushButton("Admin Panel");
        adminBtn->setStyleSheet("background-color: #ff9900; color: white; padding: 10px; border-radius: 8px; font-weight: bold;");
        sidebarLayout->addWidget(adminBtn);
    }

    // --- user info + sign out in the corner (bottom of sidebar) ---
    sidebarLayout->addStretch(1);  // push user info to bottom

    gUserLabel = new QLabel();
    gUserLabel->setStyleSheet("color: #bbbbbb; font-size: 13px;");
    gUserLabel->setText(
        QString("User: %1").arg(QString::fromStdString(currentUser.getUsername()))
    );
    sidebarLayout->addWidget(gUserLabel);

    gBalanceLabel = new QLabel();
    gBalanceLabel->setStyleSheet("color: #bbbbbb; font-size: 13px;");
    gBalanceLabel->setText(
        QString("Balance: $%1").arg(currentUser.getBalance(), 0, 'f', 2)
    );
    sidebarLayout->addWidget(gBalanceLabel);

    QPushButton *signOutBtn = new QPushButton("Sign Out");
    signOutBtn->setStyleSheet("background-color: #444444; color: white; padding: 8px; border-radius: 6px;");
    sidebarLayout->addWidget(signOutBtn);

    QWidget *sidebar = new QWidget();
    sidebar->setLayout(sidebarLayout);
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet("background-color: #181818;");

    // Stacked widget for main content
    QStackedWidget *stackedWidget = new QStackedWidget();
    QWidget *catalogPage = buildCatalogPage();
    QWidget *cartPage = buildCartPage(stackedWidget);
    QWidget *adminPage = nullptr;
    
    stackedWidget->addWidget(catalogPage); // index 0
    stackedWidget->addWidget(cartPage);    // index 1
    
    // Add admin page if user is admin
    if (currentUser.getType() == "admin") {
        adminPage = buildAdminPage(stackedWidget);
        stackedWidget->addWidget(adminPage); // index 2
    }

    // Connect buttons to switch pages
    QObject::connect(catalogBtn, &QPushButton::clicked, [stackedWidget, catalogPage]() {
        stackedWidget->setCurrentWidget(catalogPage);
    });
    
    QObject::connect(cartBtn, &QPushButton::clicked, [stackedWidget]() {
        // Rebuild cart page to refresh items
        QWidget *oldCart = stackedWidget->widget(1);
        if (oldCart) {
            stackedWidget->removeWidget(oldCart);
            delete oldCart;
        }
        QWidget *newCartPage = buildCartPage(stackedWidget);
        stackedWidget->insertWidget(1, newCartPage);
        stackedWidget->setCurrentIndex(1);
    });
    
    // Connect admin button if it exists
    if (adminBtn) {
        QObject::connect(adminBtn, &QPushButton::clicked, [stackedWidget]() {
            // Rebuild admin page to refresh user list
            QWidget *oldAdmin = stackedWidget->widget(2);
            if (oldAdmin) {
                stackedWidget->removeWidget(oldAdmin);
                delete oldAdmin;
            }
            QWidget *newAdminPage = buildAdminPage(stackedWidget);
            stackedWidget->insertWidget(2, newAdminPage);
            stackedWidget->setCurrentIndex(2);
        });
    }

    // Add sidebar to main layout
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget);

    // Show main window
    mainWindow->show();

    // Sign Out button connection
    QObject::connect(signOutBtn, &QPushButton::clicked, &app, &QApplication::quit);

    return app.exec();
}