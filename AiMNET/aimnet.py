import cv2
import sys
import os
from PIL import ImageGrab, Image
import win32gui as win32
import win32api, win32con
import numpy as np

def scrot_win(contains):
    toplist, winlist = [], []
    
    def enum_cb(hwnd, results):
        winlist.append((hwnd, win32.GetWindowText(hwnd)))

    win32.EnumWindows(enum_cb, toplist)

    hwnd, _ = [(hwnd, title) for hwnd,title in winlist if contains in title][0]
    win32.SetForegroundWindow(hwnd)
    bbox = win32.GetWindowRect(hwnd)
    img = ImageGrab.grab(bbox)

    return img, hwnd

def click_win_at(hwnd, x, y):
    wx, wy, ww, wh = win32.GetWindowRect(hwnd)
    win32api.SetCursorPos((x+wx, y+wy))


pil_img, hwnd = scrot_win(sys.argv[1] if len(sys.argv) >= 2 else 'Counter-Strike: Global Offensive')

img_raw = np.array(pil_img)
img = cv2.cvtColor(img_raw, cv2.COLOR_RGB2GRAY) # PIL uses RGB, CV uses BGR

print("Image read")
print("Loading haarcascade")

haar_cascade_face = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

print("Detecting faces")

faces = haar_cascade_face.detectMultiScale(img)

color = 255

for i, face in enumerate(faces):
    x, y, width, height = face
    print(face)
    cv2.rectangle(img, (x, y), (x + width, y + height), color=color, thickness=2)
    cv2.putText(img, f"{i}", (x, y-5), cv2.FONT_HERSHEY_SIMPLEX, 1.0, color, 2)

    if i == 0:
        facex = int(x + width / 2)
        facey = int(y + height / 2)
        click_win_at(hwnd, facex, facey)

img_rgb = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)
pil_final = Image.fromarray(img_rgb)
# pil_final.show()
