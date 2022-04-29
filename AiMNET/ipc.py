import json

def report_body(body):
	x, y, x1, y1 = map(int, body)
	width = x1 - x
	height = y1 - y

	data = {
		"type": "body",
		"x": x,
		"y": y,
		"width": width,
		"height": height
	}

	print(json.dumps(data))

def report_time(time):
	data = {
		"type": "time",
		"ms": int(round(time * 1000))
	}

	print(json.dumps(data))

def batch_done():
	data = {
		"type": "batch_done",
	}

	print(json.dumps(data))
