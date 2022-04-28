#include "Overlay.h"
#include <Windows.h>
#include <QDebug>

Overlay::Overlay(QObject *parent)
    : QObject{parent}
{
    connect(&_timer, &QTimer::timeout, this, &Overlay::updatePosition);
    _timer.start(60);
}

Overlay::~Overlay()
{
}

int Overlay::width() const
{
    return _width;
}

int Overlay::height() const
{
    return _height;
}

int Overlay::x() const
{
    return _x;
}

int Overlay::y() const
{
    return _y;
}

void Overlay::updatePosition()
{
    HWND hwnd = FindWindow(NULL, L"Microsoft Visual Studio");

    qDebug() << "Got HWND" << hwnd;

    RECT wndRect;
    if (GetWindowRect(hwnd, &wndRect))
    {
        _width = wndRect.right - wndRect.left;
        _height = wndRect.bottom - wndRect.top;
        _x = wndRect.left;
        _y = wndRect.top;

        qDebug() << "Got rect" << _width << _height << _x << _y;

        emit widthChanged();
        emit heightChanged();
        emit xChanged();
        emit yChanged();
    }
}
