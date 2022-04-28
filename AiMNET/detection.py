# import some common libraries
import numpy as np
import cv2, torch

class Detector:
	def __init__(self):
		# self.face = yolo.face_analysis()
		self.yolo = torch.hub.load('ultralytics/yolov5', 'yolov5s')
		
#		self.d2 = get_cfg()
#		self.d2.merge_from_file(model_zoo.get_config_file('COCO-InstanceSegmentation/mask_rcnn_R_50_FPN_3x.yaml'))
#		self.d2.MODEL.ROI_HEADS.SCORE_THRESH_TEST = 0.5 # threshold
#		self.d2.MODEL.WEIGHTS = model_zoo.get_checkpoint_url("COCO-InstanceSegmentation/mask_rcnn_R_50_FPN_3x.yaml")
#		self.d2.MODEL.DEVICE = "cpu"
#		self.d2_pred = DefaultPredictor(self.d2)

	# image - numpy array
#	def detect_face(self, image):
#		img, box, conf = self.face.face_detection(frame_arr=image,
#			frame_status=True,
#			model='tiny')

#		img_out = self.face.show_output(image, box, frame_status=True)
#		cv2.imshow('Output', img_out)

	# image - PIL image
	def detect_body(self, image):
		results = self.yolo(image)
		ents = results.pandas().xyxy[0]

		return list(ents[ents['name'] == 'person']['xmin ymin xmax ymax'.split(' ')].itertuples(index=False, name=None))


#	def detectron2(self, image):
#		outputs = self.d2_pred(image)
#
#		v = Visualizer(im[:, :, ::-1], MetadataCatalog.get(self.d2.DATASETS.TRAIN[0]), scale=1.2)
#		out = v.draw_instance_predictions(outputs["instances"].to("cpu"))
#		cv2.imshow(out.get_image()[:, :, ::-1])

	# image - numpy array
	def detect_opencv(self, image):
		haar_cascade_face = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

		print("Detecting faces")

		faces = haar_cascade_face.detectMultiScale(image)

		color = 255

		for i, face in enumerate(faces):
			x, y, width, height = face
			print(face)
			cv2.rectangle(img, (x, y), (x + width, y + height), color=color, thickness=2)
			cv2.putText(img, f"{i}", (x, y-5), cv2.FONT_HERSHEY_SIMPLEX, 1.0, color, 2)

			if i == 0:
				facex = int(x + width / 2)
				facey = int(y + height / 2)
				windows.click_win_at(hwnd, facex, facey)

		img_rgb = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)
		pil_final = Image.fromarray(img_rgb)
		pil_final.show()
