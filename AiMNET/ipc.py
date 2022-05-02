import json
import sys
import queue

# On small boxes, the head tends to be further down, on large ones, it's near the top.
# we can just use a fixed pixel offset that works "good enough"
head_offset = 12

def head_bounds(x, y, x1, y1):
        width = x1 - x
        height = y1 - y

        hx = int(x + width / 2)
        hy = int(y + head_offset)

        return (hx, hy)

def report_body(body):
    x, y, x1, y1 = map(int, body)
    width = x1 - x
    height = y1 - y
    head_x, head_y = head_bounds(*body)

    data = {
        "type": "body",
        "x": x,
        "y": y,
        "width": width,
        "height": height,
        "headX": head_x,
        "headY": head_y
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
    sys.stdout.flush()

def status_update(message):
    data = {
        "type": "log",
        "message": message
    }

    print(json.dumps(data))
    sys.stdout.flush()
