from PIL import ImageGrab, Image
import win32gui as win32
import win32api, win32con, win32ui
from ctypes import windll
from contextlib import contextmanager,redirect_stderr,redirect_stdout
from os import devnull

@contextmanager
def quiet():
    """A context manager that redirects stdout and stderr to devnull"""
    with open(devnull, 'w') as fnull:
        with redirect_stderr(fnull) as err, redirect_stdout(fnull) as out:
            yield (err, out)

def screenshot(contains):
    toplist, winlist = [], []
    
    def enum_cb(hwnd, results):
        winlist.append((hwnd, win32.GetWindowText(hwnd)))

    win32.EnumWindows(enum_cb, toplist)

    windll.user32.SetProcessDPIAware()

    hwnd, _ = [(hwnd, title) for hwnd,title in winlist if contains in title][0]
    # win32.SetForegroundWindow(hwnd)
    left, top, right, bottom = win32.GetWindowRect(hwnd)
    w = right - left
    h = bottom - top

    hwnd_dc = win32.GetWindowDC(hwnd)
    mfc_dc = win32ui.CreateDCFromHandle(hwnd_dc)
    save_dc = mfc_dc.CreateCompatibleDC()

    save_bitmap = win32ui.CreateBitmap()
    save_bitmap.CreateCompatibleBitmap(mfc_dc, w, h)

    save_dc.SelectObject(save_bitmap)

    result = windll.user32.PrintWindow(hwnd, save_dc.GetSafeHdc(), 2)
    print('screenshot result', result)

    bmp_info = save_bitmap.GetInfo()
    bmp_str = save_bitmap.GetBitmapBits(True)

    print(bmp_info)

    img = Image.frombuffer('RGB', (bmp_info['bmWidth'], bmp_info['bmHeight']),
        bmp_str, 'raw', 'BGRX', 0, 1)

    win32.DeleteObject(save_bitmap.GetHandle())
    save_dc.DeleteDC()
    mfc_dc.DeleteDC()
    win32.ReleaseDC(hwnd, hwnd_dc)

    return img, hwnd

# hard coded for now
SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080

def click_win_at(hwnd, x, y):
    wx, wy, ww, wh = win32.GetWindowRect(hwnd)
    win32api.SetCursorPos((x+wx, y+wy))
    win32api.mouse_event(win32con.MOUSEEVENTF_MOVE | win32con.MOUSEEVENTF_ABSOLUTE,
        int(x/SCREEN_WIDTH*65535.0), int(y/SCREEN_HEIGHT*65535.0))
