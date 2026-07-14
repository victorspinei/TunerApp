#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QIcon>
#include <QFont>
#include <QTimer>

class Window : public QMainWindow {
  public:
    Window(
        AudioHandler& handler,
        SignalProcessor& processor, 
        Tuner& tuner,
        int frames
    ) 
      : handler(handler), 
        processor(processor),
        tuner(tuner),
        frames(frames)
    {
        resize(800, 600);
        setWindowTitle("Guitar Tuner");
        setWindowIcon(QIcon("images/icon.png"));

        label = new QLabel("Waiting for audio device...", this);
        label->setAlignment(Qt::AlignCenter);
        label->setFont(QFont("Arial", 24));

        setCentralWidget(label);

        timer = new QTimer(this);
        
        connect(
            timer,
            &QTimer::timeout,
            this,
            &Window::updateTuner
        );

        timer->start(50);
    }

    void setStatus(const QString &text) {
        label->setText(text);
    }
  private:
    QLabel *label;
    QTimer *timer;

    AudioHandler& handler;
    SignalProcessor& processor;
    Tuner& tuner;

    int frames;

    void updateTuner() {
        float samples[frames];

        if(handler.read(samples, frames) < 0) {
            label->setText("Audio error");
            return;
        }

        float frequency = processor.detect(samples);


        if(frequency == 0) {
            label->setText("Pluck a string");
            return;
        }

        auto result = tuner.result(frequency);

        label->setText(
            QString("%1\n%2 Hz\n%3 cents")
            .arg(QString::fromStdString(result.note))
            .arg(result.frequency)
            .arg(result.cents)
        );
    }
};

#endif