import logging 

import pyttsx3

logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


class Speaker:
    def __init__(self):
        self.tts = pyttsx3.init()  # initialization of speach engine 
        self.voices = self.tts.getProperty('voices')

        # Default configuration
        self.current_voice = 'english'
        self.tts.setProperty('voice',  self.current_voice)
        self.tts.setProperty('rate', 150)
        self.tts.setProperty('volume', 0.8)

    def say(self, msg):
        """TTS will say the string [robotic voice]

        Args:
            msg ([str]): String to say 
        """
        self.tts.say(msg)
        self.tts.runAndWait()

    def show_all_voices(self):
        """Get the list of all available voices
        """
        for voice in self.voices:
            print(f'>>>>>>>', 
                  f'Name: {voice.name}', 
                  f'ID: {voice.id}',
                  f'Language(s): {voice.languages}',
                  f'Gender: {voice.gender}',
                  f'Age: {voice.age}'
                  )

    def change_voice(self, voice_name):
        """Change the property of choosen voice for TTS

        Args:
            voice_name ([str]): The name of voice 
        """
        flag = False 
        for voice in self.voices:
            if voice.name == voice_name:
                self.tts.setProperty('voice', voice.id)
                logger.debug(f"Voice {voice_name} was changed to  successfully!")
                flag = True 
                break 
        if not flag:
            logger.debug(f"Voice {voice_name} not found. Changing - faild!")

    def change_rate(self, new_rate):
        """Change the rate (speed) of voice 

        Args:
            new_rate ([int]): value of the new rate 
        """
        # rate in %, >100 
        self.tts.setProperty('rate', new_rate)

    def __str__(self):
        return f"Hi, I'm a Speaker! I speak {self.current_voice}."


if __name__ == '__main__':
    test = Speaker()
    # test.change_voice('Microsoft David Desktop - English (United States)')
    # test.show_all_voices()
    test.say('I am the danger')

    print(test)
