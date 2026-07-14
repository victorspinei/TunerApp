#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QIcon>
#include <QFont>

class Window : public QMainWindow {
  public:
    Window() {
        resize(800, 600);
        setWindowTitle("Guitar Tuner");
        setWindowIcon(QIcon("images/icon.png"));

        label = new QLabel("Waiting for audi device...", this);
        label->setAlignment(Qt::AlignCenter);
        label->setFont(QFont("Arial", 24));

        setCentralWidget(label);

    }

    void setStatus(const QString &text) {
        label->setText(text);
    }
  private:
  QLabel *label;
};

#endif