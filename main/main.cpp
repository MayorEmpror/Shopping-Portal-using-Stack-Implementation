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

#include "../templates/products/main.h"
#include "../data/DataArray.h"
#include "./LoginWindow.hpp"
#include "../templates/userProfile/main.h"

// Global logged‑in user
User currentUser;

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
            display: flex;
            flex-direction: row;
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
QWidget* buildCartPage() {
    QWidget *cartPage = new QWidget();
    cartPage->setStyleSheet("background-color: #202020;");

    QVBoxLayout *layout = new QVBoxLayout(cartPage);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QLabel *title = new QLabel("Your Cart");
    title->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
    layout->addWidget(title);

    // Scroll area for items
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { border: none; }");

    QWidget *container = new QWidget();
    QVBoxLayout *listLayout = new QVBoxLayout(container);
    listLayout->setSpacing(12);

    CART &cart = currentUser.getShoppingCart();
    std::stack<Products> s = cart.getItemsToBuy();

    while (!s.empty()) {
        Products p = s.top();
        s.pop();

        QFrame *itemFrame = new QFrame();
        itemFrame->setStyleSheet("background-color: #2a2a2a; border-radius: 8px;");
        itemFrame->setMinimumHeight(90);

        QHBoxLayout *h = new QHBoxLayout(itemFrame);
        h->setContentsMargins(10, 10, 10, 10);
        h->setSpacing(15);

        // Image
       // Image
QLabel *img = new QLabel();
QPixmap pix(QString::fromStdString(p.getMainImage()));
if (!pix.isNull()) {
    img->setPixmap(pix.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
img->setFixedSize(60, 60);

        // Text (name + price)
        QVBoxLayout *v = new QVBoxLayout();
        QLabel *name = new QLabel(QString::fromStdString(p.getname()));
        name->setStyleSheet("color: white; font-size: 16px;");
        QLabel *price = new QLabel("$" + QString::number(p.getprice(), 'f', 2));
        price->setStyleSheet("color: #00ff00; font-size: 14px; font-weight: bold;");

        v->addWidget(name);
        v->addWidget(price);

        h->addWidget(img);
        h->addLayout(v);
        h->addStretch();

        listLayout->addWidget(itemFrame);
    }

    listLayout->addStretch(1);

    scroll->setWidget(container);
    layout->addWidget(scroll);

    // Summary area (subtotal + checkout button)
    QFrame *summary = new QFrame();
    summary->setStyleSheet("background-color: #1c1c1c; border-radius: 12px; padding: 15px;");
    QVBoxLayout *sumLayout = new QVBoxLayout(summary);

    // Calculate total
    double total = 0;
    std::stack<Products> temp = cart.getItemsToBuy();
    while (!temp.empty()) {
        total += temp.top().getprice();
        temp.pop();
    }

    QLabel *subtotalLabel = new QLabel(QString("Subtotal: $%1").arg(total, 0, 'f', 2));
    subtotalLabel->setStyleSheet("color: white; font-size: 16px;");
    sumLayout->addWidget(subtotalLabel);

    QPushButton *checkoutBtn = new QPushButton("Checkout");
    checkoutBtn->setStyleSheet(
        "background-color: #007bff; color: white; padding: 10px; border-radius: 8px;"
    );
    sumLayout->addWidget(checkoutBtn);

    layout->addWidget(summary);

    // Checkout logic: use user's account balance
    QObject::connect(checkoutBtn, &QPushButton::clicked, []() {
        if (!currentUser.checkout()) {
            QMessageBox::warning(nullptr, "Insufficient funds",
                                 "Insufficient funds in your account to complete this purchase.");
        } else {
            QMessageBox::information(nullptr, "Success",
                                     "Payment successful! Your order is on its way.");
        }
    });

    return cartPage;
}

// ----------------------------------------------
// MAIN
// ----------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 1) Login / Sign Up dialog
    LoginWindow loginDialog;
    if (loginDialog.exec() != QDialog::Accepted) {
        return 0; // user closed dialog or cancelled
    }
    currentUser = loginDialog.getCurrentUser();

    // 2) Main shopping window
    QWidget window;
    window.setWindowTitle("Product Catalog - Qt Edition");
    window.resize(1000, 600);

    QHBoxLayout *mainLayout = new QHBoxLayout(&window);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Stacked widget to switch pages
    QStackedWidget *stack = new QStackedWidget();
    QWidget *catalogPage = buildCatalogPage();
    QWidget *cartPage    = buildCartPage();
    stack->addWidget(catalogPage);
    stack->addWidget(cartPage);

    // Sidebar (menu)
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

    sidebarLayout->addStretch(1);

    QWidget *sidebar = new QWidget();
    sidebar->setLayout(sidebarLayout);
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet("background-color: #181818;");

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);

    // Connections for switching tabs
    QObject::connect(catalogBtn, &QPushButton::clicked, [=]() {
        stack->setCurrentWidget(catalogPage);
    });

    QObject::connect(cartBtn, &QPushButton::clicked, [&, stack]( ) {
        QWidget *newCartPage = buildCartPage();   // rebuild to refresh items
        stack->removeWidget(stack->widget(1));
        stack->addWidget(newCartPage);
        stack->setCurrentWidget(newCartPage);
    });

    window.show();
    return app.exec();
}