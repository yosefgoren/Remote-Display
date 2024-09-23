import socket as sc
import time
import random

PEER = ('localhost', 8080)

s = sc.socket(sc.AF_INET, sc.SOCK_DGRAM)
while True:
	payload = bytes([random.randint(0, 255) for i in range(1200)])
	time.sleep(0.005)
	print(s.sendto(payload, PEER))