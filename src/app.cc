#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QIcon>
#include <iostream>

class Window : public QMainWindow {
public:
    Window() {
        resize(800, 600);
        setWindowTitle("Guitar Tuner");
        setWindowIcon(QIcon("images/icon.png"));

        auto *button = new QPushButton("Click me!");
        connect(button, &QPushButton::clicked, this, &Window::onButtonClicked);
        setCentralWidget(button);
    }

    void onButtonClicked() {
        std::cout << "Button clicked!" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}