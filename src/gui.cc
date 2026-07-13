#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <iostream>

class Window : public QMainWindow {
public:
    Window() {
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