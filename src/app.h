#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QFont>
#include <QTimer>

#include "tuner_widget.h"

class Window : public QMainWindow {
  public:
    Window(
        AudioHandler& handler,
        SignalProcessor& processor, 
        Tuner& tuner,
        int frames,
        float reference_pitch
    ) 
      : handler(handler), 
        processor(processor),
        tuner(tuner),
        frames(frames),
        reference_pitch(reference_pitch)
    {
        resize(800, 600);
        setWindowTitle("Guitar Tuner");
        setWindowIcon(QIcon("images/icon.png"));

        topLabel = new QLabel("Waiting...", this);
        topLabel->setAlignment(Qt::AlignCenter);
        topLabel->setFont(QFont("Arial", 18));


        needle = new TunerWidget(this);


        bottomLabel = new QLabel("", this);
        bottomLabel->setAlignment(Qt::AlignCenter);
        bottomLabel->setFont(QFont("Arial", 18));


        QVBoxLayout *layout = new QVBoxLayout;

        layout->addWidget(topLabel);
        layout->addWidget(needle);
        layout->addWidget(bottomLabel);


        QWidget *container = new QWidget(this);
        container->setLayout(layout);

        setCentralWidget(container);

        timer = new QTimer(this);
        
        connect(
            timer,
            &QTimer::timeout,
            this,
            &Window::updateTuner
        );

        timer->start(50);
    }

  private:
    QTimer *timer;

    QLabel *topLabel;
    QLabel *bottomLabel;

    TunerWidget *needle;

    AudioHandler& handler;
    SignalProcessor& processor;
    Tuner& tuner;

    int frames;
    float reference_pitch;

    void updateTuner() {
        float samples[frames];

        if(handler.read(samples, frames) < 0) {
            topLabel->setText("");
            bottomLabel->setText("Audio error");
            needle->setCents(0);
            return;
        }

        float frequency = processor.detect(samples);


        if(frequency == 0) {
            topLabel->setText(
                QString("Reference Pitch\nA: %1 Hz")
                .arg(reference_pitch)
            );

            bottomLabel->setText("Pluck a string");
            needle->setCents(0);

            return;
        }


        auto result = tuner.result(frequency);


        // top text: detected note + frequency
        topLabel->setText(
            QString("%1\n%2 Hz")
            .arg(QString::fromStdString(result.note))
            .arg(frequency, 0, 'f', 2)
        );


        // move needle
        needle->setCents(result.cents);


        // bottom text: cents difference
        bottomLabel->setText(
            QString("%1 cents")
            .arg(result.cents, 0, 'f', 1)
        );
    }
};

#endif