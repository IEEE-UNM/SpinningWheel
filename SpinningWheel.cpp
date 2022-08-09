/*
** Code inspire by
** https://stackoverflow.com/questions/26958644/qt-loading-indicator-widget
** https://github.com/mojocorp/QProgressIndicator
** https://github.com/snowwlex/QtWaitingSpinner
*/
#include "SpinningWheel.h"
#include <QPainter>
#include <iostream>
#include <QTransform>
#include <QTimer>
#include <QRandomGenerator>
#include <QRectF>
#include <QRect>
#include <QPointF>

using namespace std;

void SpinningWheel::updateItems(QStringList inputItems) {
    items = inputItems;
    this->nItems = inputItems.size();
    while (colour.size() != nItems) {
        if (colour.size() > nItems) {
            colour.pop_back();
        } else {
            colour.push_back(this->getRandomColour());
        }
    }
    this->update();
}

SpinningWheel::SpinningWheel(QWidget *parent = nullptr) {
    this->setParent(parent);

    // Adding Temporarty Item
    items << "Please Insert Item";
    nItems++;
    colour.push_back(this->getRandomColour());

    // Generating startAngle
    startAngle = QRandomGenerator::global()->bounded(5760);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setMinimumSize(500, 500);

    // Starting Timer
    timerId = startTimer(100);
}

SpinningWheel::SpinningWheel(QStringList inputItems, QWidget *parent = nullptr) {
    this->setParent(parent);
    updateItems(inputItems);
    timerId = startTimer(100);
}

QStringList SpinningWheel::getItems() {
    return items;
}

void SpinningWheel::spin() {
    killTimer(timerId);
    timerId = startTimer(1);
    spinRatio = 5;
    QTimer::singleShot(1000, this, &SpinningWheel::stopEvent);
}

void SpinningWheel::stop() {
    killTimer(timerId);
    QString currentItem = getCurrentItem();
    emit stopped(currentItem);
}

QString SpinningWheel::getCurrentItem() {
    int turnedAngle = - startAngle % 5760;
    int itemIndex = turnedAngle / spanAngle;
    return items.at(itemIndex);
}

void SpinningWheel::timerEvent(QTimerEvent *event) {
    // Stopping Timer
    if (event->timerId() != timerId) {
        spinRatio -= 1;
        if (spinRatio == 0) {
            killTimer(event->timerId());
            stop();
        }
        return;
    }

    // Spinning Timer
    startAngle -= 1 * 16 * spinRatio;
    this->update();
}

void SpinningWheel::paintEvent(QPaintEvent *) {
    // Setting up painter
    QPainter painter(this);

    // Getting the shorter side of the widget
    QSize size = this->size();
    int width = size.width();
    int height = size.height();
    int maxValue;
    if (width > height) {
        maxValue = height;
    } else {
        maxValue = width;
    }

    // Getting Font Size
    const int DPI = this->screen()->logicalDotsPerInch();
    // Getting pixels per point
    const float pxPerPoint = (float) DPI / 72;
    const int fontSize = painter.font().pointSize();
    const int pixelSize = pxPerPoint * fontSize;

    // Getting needed values
    double padding = 0.1;
    unsigned int slices = this->nItems;
    if (slices == 0) {
        items << "Please Insert Item";
        colour.push_back(this->getRandomColour());
        slices = 1;
    }
    float topCornerValue = padding * maxValue;
    float bottomCornerValue = (1 - padding * 2) * maxValue; // width and height
    float centerValue = bottomCornerValue / 2 + topCornerValue;
    float radius = bottomCornerValue / 2;

    // Getting Values for slices
    QRectF rectangle(topCornerValue, topCornerValue,
                     bottomCornerValue, bottomCornerValue);
    spanAngle = 360 / slices * 16;

    // Drawing Pies
    for (unsigned int i = 0; i < slices; i++) {
        painter.setBrush(colour[i]);
        painter.drawPie(rectangle, -(i * (spanAngle) + startAngle), spanAngle);
    }
    // Drawing Text
    // Initial Rotation
    painter.translate(centerValue, centerValue);
    painter.rotate(startAngle / 16 + spanAngle / 32);
    painter.translate(-centerValue, -centerValue);
    for (unsigned int i = 0; i < slices; i++) {
        QRect boundingRect;
        QRect textRect(centerValue, centerValue - pixelSize / 2,
                       radius * (1 - padding), pixelSize);
        painter.drawText(textRect, Qt::AlignRight, items[i], &boundingRect);

        // Setting up Transformation
        painter.translate(centerValue, centerValue);
        painter.rotate(spanAngle / 16);
        painter.translate(-centerValue, -centerValue);
    }

    // Pointg Arrow
    painter.resetTransform();
    painter.setBrush(Qt::black);
    QPointF triangle[3] = {
        QPointF(centerValue + radius, centerValue),
        QPointF(maxValue, centerValue + centerValue * padding / 2),
        QPointF(maxValue, centerValue - centerValue * padding / 2),
    };
    painter.drawConvexPolygon(triangle, 3);
}

void SpinningWheel::stopEvent() {
    startTimer(1000);
}

QColor SpinningWheel::getRandomColour() {
    int coloursSize = sizeof(colours) / sizeof(QColor);
    int randomInt = QRandomGenerator::global()->bounded(coloursSize);
    return colours[randomInt];
}
