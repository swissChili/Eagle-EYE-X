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

bool Overlay::hasFocus() const
{
    return _hasFocus;
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
    if (GetClientRect(hwnd, &wndRect))
    {
        POINT clientPt = {0, 0};

        if (ClientToScreen(hwnd, &clientPt))
        {
            wndRect.left += clientPt.x;
            wndRect.right += clientPt.x;
            wndRect.top += clientPt.y;
            wndRect.bottom += clientPt.y;
        }

        _width = wndRect.right - wndRect.left;
        _height = wndRect.bottom - wndRect.top;
        _x = wndRect.left;
        _y = wndRect.top;

        emit widthChanged();
        emit heightChanged();
        emit xChanged();
        emit yChanged();
    }

    bool nextFocus = GetActiveWindow() == hwnd;

    if (nextFocus != _hasFocus)
    {
        qDebug() << _hasFocus << nextFocus;

        _hasFocus = nextFocus;
        emit hasFocusChanged();
    }
}
