#ifndef SPINNINGWHEEL_H_
#define SPINNINGWHEEL_H_

#include <QWidget>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QStringList>
#include <QColor>
#include <vector>

const QColor colours[] = {
    Qt::cyan,
    Qt::darkCyan,
    Qt::red,
    Qt::darkRed,
    Qt::magenta,
    Qt::darkMagenta,
    Qt::green,
    Qt::darkGreen,
    Qt::yellow,
    Qt::darkYellow,
    Qt::blue,
    Qt::darkBlue,
    Qt::gray,
    Qt::darkGray,
    Qt::lightGray,
};

class SpinningWheel: public QWidget {
    Q_OBJECT
    public:
        void updateItems(QStringList);
        SpinningWheel(QWidget *);
        SpinningWheel(QStringList, QWidget *);
        QStringList getItems();
    public slots:
        void spin();
        void stop();
        QString getCurrentItem();
    signals:
        void stopped(QString);
    protected:
        void timerEvent(QTimerEvent *);
        void paintEvent(QPaintEvent *paintEvent);
        void stopEvent();
        unsigned int nItems = 0;
        QStringList items;
        double spinRatio = 1;
        int timerId;
        std::vector<QColor> colour;
    private:
        int startAngle = 0;
        int spanAngle = 0;
        QColor getRandomColour();
};
#endif // SPINNINGWHEEL_H_
