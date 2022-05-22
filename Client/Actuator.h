#pragma once

#include <QObject>
#include "Overlay.h"
#include <qqml.h>
#include <QThread>
#include <QTime>

class ActuatorWorker : public QObject
{
    Q_OBJECT
public:
    ActuatorWorker(QObject *parent = nullptr);

    ~ActuatorWorker();

public slots:
    void moveInParts(int dx, int dy);

    void shootAt(int x, int y, int w, int h);

protected:
    double multiplier();

private:
    int _overallTime = 100;
    int _moveInterval = 60;
    double _sens = 1.7;

    QThread *_thread;
};

class Actuator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)

    QML_ELEMENT

public:
    explicit Actuator(QObject *parent = nullptr);
    ~Actuator();

    int width() const;
    int height() const;

    void setWidth(int width);
    void setHeight(int height);

public slots:
    // x and y are absolute screen coordinates
    Q_INVOKABLE void shootAt(int x, int y);

signals:
    void widthChanged();
    void heightChanged();
    void shouldShootAt(int x, int y, int w, int h);

private:
    int _width = 0, _height = 0;
    int _minTimeBetweenShots = 200;
    QTime _lastShot = QTime::currentTime();
    ActuatorWorker *_worker;
};
