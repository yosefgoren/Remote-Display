import socket as sc
import time
import random

PEER = ('172.26.112.1', 8080)

s = sc.socket(sc.AF_INET, sc.SOCK_DGRAM)
while True:
	payload = bytes([random.randint(0, 255) for i in range(1440)])
	time.sleep(0.04)
	print(s.sendto(payload, PEER))