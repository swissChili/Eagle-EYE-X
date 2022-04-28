import cv2
import sys
import os
from PIL import ImageGrab, Image
import numpy as np

import windows
import detection

def head_bounds(x, y, x1, y1):
	width = x1 - x
	height = y1 - y

	hx = int(x + width / 2)
	hy = int(y + height / 5)

	return (hx, hy)

window_title = 'Counter-Strike: Global Offensive'
if len(sys.argv) >= 2:
    window_title = sys.argv[1]

det = detection.Detector()

print('Taking screenshot')
pil_img, hwnd = windows.screenshot(window_title)

img_raw = np.array(pil_img)
img = cv2.cvtColor(img_raw, cv2.COLOR_RGB2BGR) # PIL uses RGB, CV uses BGR

# det.detect_face(img)
bodies = det.detect_body(pil_img)

color = (255, 0, 0)

for i, body in enumerate(bodies):
	x, y, x1, y1 = map(int, body)
	width = x1 - x
	height = y1 - y

	cv2.rectangle(img, (x, y), (x1, y1), color=color, thickness=2)
	cv2.circle(img, head_bounds(x, y, x1, y1), 10, color, 2)
	cv2.putText(img, f"Enemy", (x, y-5), cv2.FONT_HERSHEY_SIMPLEX, 1.0, color, 2)

	if i == 0:
		facex = int(x + width / 2)
		facey = int(y + height / 2)
		# windows.click_win_at(hwnd, facex, facey)

img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
pil_final = Image.fromarray(img_rgb)
pil_final.show()
