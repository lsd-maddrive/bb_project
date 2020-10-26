
import sys 
sys.path.append("../")
import time

from src.xbox_one import Joystick

joy = Joystick()
joy.close()
joy = Joystick()
# time.sleep(5)
for i in range(20):
	time.sleep(0.1)
	print(joy.leftTrigger())