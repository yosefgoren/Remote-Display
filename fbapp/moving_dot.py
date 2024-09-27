#!/usr/bin/python3

import torch
import time

HEIGHT_PX = 20
WIDTH_PX = 24
BPP = 4 # Bytes Per Pixel

BLUE_CHANNEL = 0
GREEN_CHANNEL = 1
RED_CHANNEL = 2

with open("/dev/fb1", 'wb') as f:
    def write_fb(frame: torch.Tensor):
        f.seek(0)
        frame.numpy().tofile(f)

    t = torch.zeros(HEIGHT_PX, WIDTH_PX, BPP, dtype=torch.uint8)

    while True:
        for i in range(WIDTH_PX):
            for j in range(HEIGHT_PX):
                t[j, i, RED_CHANNEL] = 255
                write_fb(t)
                time.sleep(0.03)
                t[j, i, RED_CHANNEL] = 0
            
