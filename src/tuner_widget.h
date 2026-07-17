#ifndef TUNER_WIDGET_H
#define TUNER_WIDGET_H

#include <QWidget>
#include <QPainter>
#include <cmath>

class TunerWidget : public QWidget {
public:
    TunerWidget(QWidget *parent = nullptr)
        : QWidget(parent), cents(0)
    {
        setMinimumSize(600, 300);
    }

    void setCents(float value) {
        cents = value;
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        int w = width();
        int h = height();

        QPoint center(w / 2, h);

        // draw arc
        QRectF arcRect(
            w / 4,
            h / 4,
            w / 2,
            h / 2
        );

        painter.drawArc(
            arcRect,
            30 * 16,
            120 * 16
        );


        float displayCents = std::clamp(cents, -50.0f, 50.0f);
        float angle = -displayCents * 1.2f;

        painter.save();

        painter.translate(center);
        painter.rotate(-angle);


        // needle
        painter.drawLine(
            0,
            0,
            0,
            -h / 3
        );

        painter.restore();


        // center line
        painter.drawLine(
            w/2,
            h/2,
            w/2,
            h/2 + 20
        );
    }

private:
    float cents;
};

#endif