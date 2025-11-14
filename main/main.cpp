#include <QApplication>
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QStyleOption>
#include <QPainter>

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

QFrame* createProductCard(const QString &title, const QString &price, const QString &desc) {
    QFrame *card = new QFrame();
    card->setStyleSheet(R"(
        QFrame {
            background-color: #1c1c1c;
            border-radius: 12px;
            padding: 10px;
        }
        QLabel {
            color: #ffffff;
        }
    )");

    QVBoxLayout *vbox = new QVBoxLayout(card);

    QLabel *img = new QLabel();
    img->setFixedSize(100, 100);
    img->setStyleSheet("background-color: #333333; border-radius: 10px;");
    img->setAlignment(Qt::AlignCenter);
    img->setText("🪴");

    QLabel *name = new QLabel("<b>" + title + "</b>");
    QLabel *descLabel = new QLabel(desc);
    QLabel *priceLabel = new QLabel(price);

    vbox->addWidget(img, 0, Qt::AlignCenter);
    vbox->addWidget(name, 0, Qt::AlignCenter);
    vbox->addWidget(descLabel, 0, Qt::AlignCenter);
    vbox->addWidget(priceLabel, 0, Qt::AlignCenter);

    return card;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            font-family: "Segoe UI";
        }
        QPushButton {
            background-color: #292929;
            color: #ffffff;
            border-radius: 8px;
            padding: 8px 12px;
        }
        QPushButton:hover {
            background-color: #3a3a3a;
        }
        QLabel {
            color: #cccccc;
        }
    )");

    QWidget window;
    window.setWindowTitle("Plantify Catalog - Qt Edition");
    window.resize(900, 600);

    // Layouts
    QHBoxLayout *mainLayout = new QHBoxLayout(&window);

    // Sidebar (filters)
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    QLabel *filtersLabel = new QLabel("<b>Filters</b>");
    filtersLabel->setStyleSheet("color: #ffffff; font-size: 18px;");
    sidebarLayout->addWidget(filtersLabel);

    QPushButton *cat1 = new QPushButton("Potted Elegance");
    QPushButton *cat2 = new QPushButton("Outdoor Charm");
    QPushButton *cat3 = new QPushButton("Compact Decor");
    sidebarLayout->addWidget(cat1);
    sidebarLayout->addWidget(cat2);
    sidebarLayout->addWidget(cat3);
    sidebarLayout->addStretch(1);

    QWidget *sidebar = new QWidget();
    sidebar->setLayout(sidebarLayout);
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet("background-color: #181818; border-radius: 12px;");

    // Catalog area
    QWidget *catalog = new QWidget();
    QGridLayout *grid = new QGridLayout(catalog);
    grid->setSpacing(20);

    QStringList names = {"Tropical Breeze Set", "Cozy Corner Plant", "Fresh Vibe Pot", "Serene Space Set", "Golden Hour Leaf", "Minimalist Luxe Pot"};
    QStringList prices = {"$180", "$240", "$120", "$120", "$80", "$180"};
    QStringList descs = {"Lush & vibrant", "Rich green texture", "Glossy foliage charm", "Calming deep green", "Warm leaf tones", "Clean modern lines"};

    for (int i = 0; i < names.size(); ++i) {
        QFrame *card = createProductCard(names[i], prices[i], descs[i]);
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
