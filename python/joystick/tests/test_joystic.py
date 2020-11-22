
import sys
sys.path.append("../")
import time

from src.xbox_one import Joystick

joy = Joystick()
time.sleep(5)
print('start')
while joy.left_trigger() < 0.8:
    time.sleep(0.1)
    joy.update_buttons()
    if joy.right_trigger() > 0.5:
        print(joy.x_button())
