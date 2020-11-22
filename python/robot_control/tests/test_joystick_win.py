import sys
import time
sys.path.append("../../")
from robot_control.src.xbox_win import XInputJoystick

joy = XInputJoystick(0)

go = 1

@joy.event
def on_button(number, pressed):
    if pressed:
        print(number)
        if number == 5:
            global go
            go = 0


while go:
    joy.dispatch_events()
    time.sleep(0.01)