from PIL import ImageGrab, Image
import win32gui as win32
import win32api, win32con, win32ui
from ctypes import windll
from contextlib import contextmanager,redirect_stderr,redirect_stdout
from os import devnull
import sys

@contextmanager
def quiet():
    """A context manager that redirects stdout and stderr to devnull"""
    with open(devnull, 'w') as fnull:
        with redirect_stdout(fnull) as out:
            yield out

def screenshot(contains):
    toplist, winlist = [], []
    
    def enum_cb(hwnd, results):
        winlist.append((hwnd, win32.GetWindowText(hwnd)))

    win32.EnumWindows(enum_cb, toplist)

    windll.user32.SetProcessDPIAware()

    hwnd, _ = [(hwnd, title) for hwnd,title in winlist if contains in title][0]

    # This gets the client area rectangle
    left, top, right, bottom = win32.GetClientRect(hwnd)
    # screen
    sleft, stop, sright, sbottom = win32.GetWindowRect(hwnd)
    # client origin
    co_x, co_y = win32.ClientToScreen(hwnd, (0, 0))

    offset_x = (left + co_x) - sleft
    offset_y = (top + co_y) - stop

    full_w = sright - sleft
    full_h = sbottom - stop

    w = right - left
    h = bottom - top

    hwnd_dc = win32.GetWindowDC(hwnd)
    mfc_dc = win32ui.CreateDCFromHandle(hwnd_dc)
    save_dc = mfc_dc.CreateCompatibleDC()

    save_bitmap = win32ui.CreateBitmap()
    save_bitmap.CreateCompatibleBitmap(mfc_dc, full_w, full_h)

    save_dc.SelectObject(save_bitmap)

    result = windll.user32.PrintWindow(hwnd, save_dc.GetSafeHdc(), 2)
    print('screenshot result', result)

    bmp_info = save_bitmap.GetInfo()
    bmp_str = save_bitmap.GetBitmapBits(True)

    print(bmp_info)

    img = Image.frombuffer('RGB', (bmp_info['bmWidth'], bmp_info['bmHeight']),
                           bmp_str, 'raw', 'BGRX', 0, 1)
    img = img.crop((offset_x, offset_y, offset_x + w, offset_y + h))

    print('cropping', offset_x, offset_y, offset_x + w, offset_y + h, file=sys.stderr)

    win32.DeleteObject(save_bitmap.GetHandle())
    save_dc.DeleteDC()
    mfc_dc.DeleteDC()
    win32.ReleaseDC(hwnd, hwnd_dc)

    return img, hwnd
