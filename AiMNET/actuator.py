import win32api
import win32con
import win32gui as win32
import time
import math

sensitivity = 1.7
ratio = 2.3 # 1.84
multiplier = 4.9 / sensitivity ** 1.5

game_w = 1280
game_h = 960

screen_w = 1920
screen_h = 1080

def move_in_parts(x, y):
    real_x, real_y = 0, 0
    overall_time = 0.004
    interval = 60

    time_per_interval = overall_time / (max(abs(x - real_x), abs(y - real_y)) / interval)

    while real_x < abs(x) and real_y < abs(y):
        x_int = min(x - real_x, interval)
        y_int = min(y - real_y, interval)

        win32api.mouse_event(win32con.MOUSEEVENTF_MOVE, int(x_int), int(y_int), 0, 0)
        time.sleep(time_per_interval)

        real_x += x_int
        real_y += y_int

# x and y are relative to mouse pos
def shoot_at(x, y):
    x *= screen_w / game_w
    y *= screen_w / game_w

    x = int(x * multiplier)
    y = int(y * multiplier)

    move_in_parts(x, y)
    time.sleep(0.15)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN, x, y, 0, 0)
    time.sleep(0.03)
    win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP, x, y, 0, 0)
