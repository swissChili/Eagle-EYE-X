import win32api
import win32con
import time

sensitivity = 1.3

# x and y are screen-space coordinates
def shoot_at(x, y):
    x = int(x * sensitivity)
    y = int(y * sensitivity)

    win32api.mouse_event(win32con.MOUSEEVENTF_MOVE, x, y, 0, 0)
    tims.sleep(0.05);
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN, x, y, 0, 0)
    time.sleep(0.05)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP, x, y, 0, 0)
