import xbox_one
import time

joy = xbox_one.Joystic()

while True:
	time.sleep(0.1)
	print(joy.rightTrigger())