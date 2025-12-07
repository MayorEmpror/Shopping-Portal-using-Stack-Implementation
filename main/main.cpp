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

#include "../templates/products/main.h"
#include "../data/DataArray.h"
#include "./LoginWindow.hpp"
#include "./UserStorage.hpp"
#include "../templates/userProfile/main.h"

// Global logged‑in user
User currentUser;
QLabel *gUserLabel   = nullptr;
QLabel *gBalanceLabel = nullptr;

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

    // Checkout logic
    QObject::connect(checkoutBtn, &QPushButton::clicked, []() {
        if (!currentUser.checkout()) {
            QMessageBox::warning(nullptr, "Insufficient funds",
                                 "Insufficient funds in your account to complete this purchase.");
        } else {
            QMessageBox::information(nullptr, "Success",
                                     "Payment successful! Your order is on its way.");

            // Save updated user balance to file
            UserStorage::saveUser(currentUser);

            // Refresh balance label
            if (gBalanceLabel) {
                gBalanceLabel->setText(
                    QString("Balance: $%1").arg(currentUser.getBalance(), 0, 'f', 2)
                );
            }
        }
    });

    // Add both sides to main layout
    mainLayout->addWidget(itemsContainer, 2);  // Items take 2/3 of space
    mainLayout->addWidget(summaryContainer, 1); // Summary takes 1/3

    return cartPage;
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
    stackedWidget->addWidget(catalogPage); // index 0
    stackedWidget->addWidget(cartPage);    // index 1

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

    // Add sidebar to main layout
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget);

    // Show main window
    mainWindow->show();

    // Sign Out button connection
    QObject::connect(signOutBtn, &QPushButton::clicked, &app, &QApplication::quit);

    return app.exec();
}