import sys
import time
import os
sys.path.append("../../")
from speaker.speaker import Speaker
from speaker.speaker_without_tts import Speaker2


# pyttsx3
voice = Speaker()
voice.change_voice('Microsoft David Desktop - English (United States)')
voice.say("I block thread till I finish")
print('Look how long it took!')

time.sleep(1)

# new python porcess
os.popen(" ".join(["python", "../../speaker/speaker_reworked.py", '"Thats better, but now I lags a bit"']))
print("text first YAY!")

time.sleep(5)

# direct driver access
bad_voice = Speaker2()
bad_voice.say('Sounds not as goo, but previous guy are Windows exclusive anyvay')
print("Perfect synchronisation!")
time.sleep(5)

# A bit of problem
os.popen(" ".join(["python", "../../speaker/speaker_reworked.py", '"don\'t stop me now i\'m having such a good time"']))
bad_voice.say('don\'t stop me now i\'m having such a good time')
print("Choir singing is not the strongest side of robots")