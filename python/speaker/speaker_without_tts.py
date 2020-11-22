import os


class Speaker2:
    def __init__(self):

        # Default configuration
        self.current_voice = 'en-us'
        self.speed = 150
        self.volume = 100

    def say(self, msg):
        os.popen(f'espeak-ng -v "{self.current_voice}" -a {self.volume} -s {self.speed} "{msg}"')


