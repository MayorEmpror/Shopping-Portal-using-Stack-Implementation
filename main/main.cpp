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
#include <QStyleOption>
#include <QPainter>
#include <vector>
#include "../templates/products/main.h"  // your Products class header
#include "../data/DataArray.h"
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

// Create a product card using the Products class
QFrame* createProductCard(const Products& product) {
    QFrame *card = new QFrame();
    card->setFixedSize(220, 300);
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
    )");

    QVBoxLayout *vbox = new QVBoxLayout(card);
    vbox->setAlignment(Qt::AlignTop);

    // Image
    QLabel *img = new QLabel();
    img->setFixedSize(150, 150);
    img->setAlignment(Qt::AlignCenter);
    QPixmap pix(QString::fromStdString(product.getMainImage())); // use image URL/path
    if (!pix.isNull()) {
        img->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        img->setStyleSheet("background-color: #333333; border-radius: 10px;");
        img->setText("🖼"); // fallback
    }

    QLabel *name = new QLabel(QString::fromStdString(product.getname()));
    name->setWordWrap(true);

    QLabel *price = new QLabel("$" + QString::number(product.getprice()));
    price->setStyleSheet("font-weight:bold; color:#00ff00;");

    QLabel *features = new QLabel(QString::fromStdString(product.getfeatures()));
    features->setWordWrap(true);
    features->setStyleSheet("font-size: 11px; color:#cccccc;");

    vbox->addWidget(img, 0, Qt::AlignCenter);
    vbox->addWidget(name, 0, Qt::AlignCenter);
    vbox->addWidget(price, 0, Qt::AlignCenter);
    vbox->addWidget(features, 0, Qt::AlignCenter);

    return card;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Product Catalog - Qt Edition");
    window.resize(1000, 600);

    QHBoxLayout *mainLayout = new QHBoxLayout(&window);

    // Sidebar
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    QLabel *filtersLabel = new QLabel("<b>Filters</b>");
    filtersLabel->setStyleSheet("color: #ffffff; font-size: 18px;");
    sidebarLayout->addWidget(filtersLabel);

    QStringList categories = {"Electronics", "Computers", "Audio", "Gaming", "Kitchen"};
    for (const QString &cat : categories) {
        QPushButton *btn = new QPushButton(cat);
        btn->setStyleSheet("background-color: #292929; color: #fff; border-radius: 8px; padding: 8px;");
        sidebarLayout->addWidget(btn);
    }
    sidebarLayout->addStretch(1);

    QWidget *sidebar = new QWidget();
    sidebar->setLayout(sidebarLayout);
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet("background-color: #181818; border-radius: 12px; padding: 10px;");

    // Catalog
    QWidget *catalog = new QWidget();
    QGridLayout *grid = new QGridLayout(catalog);
    grid->setSpacing(20);
    grid->setContentsMargins(10, 10, 10, 10);

    // Create products (use all 30 from your InitDBFrame)

    std::vector<Products> productList = InitDBFrame();

    // Add all product cards
    for (size_t i = 0; i < productList.size(); ++i) {
        QFrame *card = createProductCard(productList[i]);
        grid->addWidget(card, i / 3, i % 3);
    }

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(catalog);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(scrollArea);

    window.show();
    return app.exec();
}
