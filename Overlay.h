#pragma once

#include <qqml.h>
#include <QObject>
#include <QTimer>

class Overlay : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int width READ width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)
    Q_PROPERTY(int x READ x NOTIFY xChanged)
    Q_PROPERTY(int y READ y NOTIFY yChanged)

public:
    explicit Overlay(QObject *parent = nullptr);
    ~Overlay();

    int width() const;
    int height() const;
    int x() const;
    int y() const;

signals:
    void widthChanged();
    void heightChanged();
    void xChanged();
    void yChanged();

private slots:
    void updatePosition();

private:
    QString _windowName;
    int _width = 10, _height = 10, _x = 0, _y = 0;
    QTimer _timer;
};
