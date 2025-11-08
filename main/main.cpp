#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Qt GUI Window Example");
    window.resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *label = new QLabel("Hello, Qt GUI!");
    layout->addWidget(label);

    QPushButton *button = new QPushButton("Click Me");
    layout->addWidget(button);

    QObject::connect(button, &QPushButton::clicked, [&]() {
        label->setText("Button Clicked!");
    });

    window.setLayout(layout);
    window.show();

    return app.exec();
}
