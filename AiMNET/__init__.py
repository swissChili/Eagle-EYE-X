import cv2
import sys
import os

if len(sys.argv) < 2:
	print("Please give me an argument")
	os.exit(1)

print("Reading", sys.argv[1])

img_raw = cv2.imread(sys.argv[1])
img = cv2.cvtColor(img_raw, cv2.COLOR_BGR2GRAY)

haar_cascade_face = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
faces = haar_cascade_face.detectMultiScale(img)

for face in faces:
	x, y, width, height = face
	print(face)
