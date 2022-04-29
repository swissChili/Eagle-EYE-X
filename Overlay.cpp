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

static wchar_t buffer[256];

static BOOL CALLBACK Overlay_windEnumFunc(HWND testHwnd, HWND *storeHwnd)
{
    GetWindowText(testHwnd, (LPWSTR)buffer, 256);

    QString string = QString::fromWCharArray(buffer);

    if (string.startsWith("Counter-Strike: Global Offensive"))
    {
        *storeHwnd = testHwnd;
        return FALSE;
    }

    return TRUE;
}

void Overlay::updatePosition()
{
    static HWND hwnd = nullptr;

    if (hwnd == nullptr)
    {
        EnumWindows((WNDENUMPROC)Overlay_windEnumFunc, (LPARAM)&hwnd);
    }

    RECT wndRect;
    if (GetWindowRect(hwnd, &wndRect))
    {
        _width = wndRect.right - wndRect.left;
        _height = wndRect.bottom - wndRect.top;
        _x = wndRect.left;
        _y = wndRect.top;

        emit widthChanged();
        emit heightChanged();
        emit xChanged();
        emit yChanged();
    }
}
