import sys
sys.path.append("../../")
from robot_control.src.xbox_win import XInputJoystick
import time
from speaker.speaker import Speaker

joy = XInputJoystick(0)
print(joy.device_number)
run = 1

voice = Speaker()
voice.change_voice('Microsoft David Desktop - English (United States)')

@joy.event
def on_button(button, pressed):
    if pressed:
        print('button', button)
        if button == 13:
            voice.say('Hi Hi')


while run:
    joy.dispatch_events()
    time.sleep(0.01)