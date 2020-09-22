
import sys 
sys.path.append("../")
import time

import src.xbox_one


joy = xbox_one.Joystic()

while True:
	time.sleep(0.1)
	print(joy.rightTrigger())