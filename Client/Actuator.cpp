#include "Actuator.h"
#include <QThread>
#include <Windows.h>
#include <QtMath>
#include <QPoint>

Actuator::Actuator(QObject *parent)
    : QObject{parent}
    , _worker(new ActuatorWorker())
{
    connect(this, &Actuator::shouldShootAt, _worker, &ActuatorWorker::shootAt);
}

Actuator::~Actuator()
{
    delete _worker;
}

int Actuator::width() const
{
    return _width;
}

int Actuator::height() const
{
    return _height;
}

void Actuator::setWidth(int width)
{
    _width = width;
    emit widthChanged();
}

void Actuator::setHeight(int height)
{
    _height = height;
    emit heightChanged();
}

void Actuator::shootAt(int x, int y)
{
    if (_lastShot.msecsTo(QTime::currentTime()) > _minTimeBetweenShots)
    {
        emit shouldShootAt(x, y, _width, _height);
        _lastShot = QTime::currentTime();
    }
}

ActuatorWorker::ActuatorWorker(QObject *parent)
    : QObject(nullptr)
    , _thread(new QThread)
{
    moveToThread(_thread);
    _thread->start();
}

ActuatorWorker::~ActuatorWorker()
{
    _thread->terminate();
    delete _thread;
}

void ActuatorWorker::moveInParts(int dx, int dy)
{
    int realX = 0, realY = 0;

    // Unreadable
    int timePerInterval = _overallTime / qMax(qMax(qAbs(dx - realX), qAbs(dy - realY)) / _moveInterval, 1);

    while (qAbs(realX) < qAbs(dx) && qAbs(realY) < qAbs(dy))
    {
        int xInt = qMin(dx - realX, _moveInterval);
        int yInt = qMin(dy - realY, _moveInterval);

        mouse_event(MOUSEEVENTF_MOVE, xInt, yInt, 0, 0);
        _thread->msleep(timePerInterval);

        realX += xInt;
        realY += yInt;
    }
}

void ActuatorWorker::shootAt(int x, int y, int w, int h)
{
    int cursorX = w / 2;
    int cursorY = h / 2;

    QPoint pt = {x - cursorX, y - cursorY};
    pt *= multiplier();

     moveInParts(pt.x(), pt.y());

//    mouse_event(MOUSEEVENTF_MOVE, pt.x(), pt.y(), 0, 0);
//    _thread->msleep(150);
    mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x(), pt.y(), 0, 0);
    _thread->msleep(30);
    mouse_event(MOUSEEVENTF_LEFTUP, pt.x(), pt.y(), 0, 0);
}

double ActuatorWorker::multiplier()
{
    return 4.9 / qPow(_sens, 1.5);
}
