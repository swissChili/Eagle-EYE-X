# import some common libraries
import numpy as np
#import cv2
import torch
import torch_hub
# import onnxruntime
from PIL import Image
import time

yolov3_labels =["person","bicycle","car","motorbike","aeroplane","bus","train","truck","boat",
    "traffic light","fire hydrant","stop sign","parking meter","bench","bird","cat",
    "dog","horse","sheep","cow","elephant","bear","zebra","giraffe","backpack","umbrella",
    "handbag","tie","suitcase","frisbee","skis","snowboard","sports ball","kite","baseball bat",
    "baseball glove","skateboard","surfboard","tennis racket","bottle","wine glass","cup","fork",
    "knife","spoon","bowl","banana","apple","sandwich","orange","broccoli","carrot","hot dog",
    "pizza","donut","cake","chair","sofa","pottedplant","bed","diningtable","toilet","tvmonitor",
    "laptop","mouse","remote","keyboard","cell phone","microwave","oven","toaster","sink",
    "refrigerator","book","clock","vase","scissors","teddy bear","hair drier","toothbrush"]

def at_edge(rect, pos):
    x, y, x1, y1 = rect
    w, h = pos

    return y1 > h - 20

class Detector:
    def __init__(self):
        # self.face = yolo.face_analysis()
        self.yolo = torch_hub.load('ultralytics/yolov5', 'yolov5s')
        #self.onnx = onnxruntime.InferenceSession("yolov3.onnx", providers=['DmlExecutionProvider'])
        #self.inname = [input.name for input in self.onnx.get_inputs()]
        #self.outname = [output.name for output in self.onnx.get_outputs()]

    def letterbox_image(self, image, size):
        iw, ih = image.size
        w, h = size
        scale = min(w/iw, h/ih)
        nw = int(iw*scale)
        nh = int(ih*scale)

        image = image.resize((nw,nh), Image.BICUBIC)
        new_image = Image.new('RGB', size, (128,128,128))
        new_image.paste(image, ((w-nw)//2, (h-nh)//2))
        return new_image

    def preprocess(self, img):
        model_image_size = (416, 416)
        boxed_image = self.letterbox_image(img, tuple(reversed(model_image_size)))
        image_data = np.array(boxed_image, dtype='float32')
        image_data /= 255.
        image_data = np.transpose(image_data, [2, 0, 1])
        image_data = np.expand_dims(image_data, 0)
        return image_data

    # image - PIL image
    def detect_body(self, image):
        results = self.yolo(image)
        ents = results.pandas().xyxy[0]

        all_bodies = list(ents[ents['name'] == 'person']['xmin ymin xmax ymax'.split(' ')].itertuples(index=False, name=None))
        return [body for body in all_bodies if not at_edge(body, image.size)]

    # image - PIL image
#	def detect_body(self, image):
#		image_data = self.preprocess(image)
#		image_size = np.array([image.size[1], image.size[0]], dtype=np.float32).reshape(1, 2)

#		input = {
#			self.inname[0]: image_data,
#			self.inname[1]: image_size
#		}

#		t0 = time.time()
#		boxes, scores, indices = self.onnx.run(self.outname, input)
#		t1 = time.time()
#		print("Took", t1-t0)
#		for box, score, index in zip(boxes[0], scores[0], indices[0]):
#			print('box', box)
#			print('score', score)
#			print('class', yolov3_labels[index])

    # image - numpy array
#    def detect_opencv(self, image):
#        haar_cascade_face = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

#        print("Detecting faces")

#        faces = haar_cascade_face.detectMultiScale(image)

#        color = 255

#        for i, face in enumerate(faces):
#            x, y, width, height = face
#            print(face)
#            cv2.rectangle(img, (x, y), (x + width, y + height), color=color, thickness=2)
#            cv2.putText(img, f"{i}", (x, y-5), cv2.FONT_HERSHEY_SIMPLEX, 1.0, color, 2)

#            if i == 0:
#                facex = int(x + width / 2)
#                facey = int(y + height / 2)
#                windows.click_win_at(hwnd, facex, facey)

#        img_rgb = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)
#        pil_final = Image.fromarray(img_rgb)
#        pil_final.show()
