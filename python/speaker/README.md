# Let the robot speak 

First of all we need a software interface to convert text to speech on the speakers. For this we need ***a Text To Speech engine**. The TTS engine is used here is **eSpeak**.  The voice may be a little robotic, however it runs offline.

1. First let’s test if the audio is working on the Raspberry Pi (or any other PCs).  Run the following command:

```hash
aplay /usr/share/sounds/alsa/*
```

> If you are able to hear the sounds like “Front Center”,”Front Left”, “Front Right” and so on, your sound is working!  

2. Next, install **eSpeak**.  Run the following in terminal to install espeak:

```bash
sudo apt-get install espeak-ng python-espeak
```

3. Download and unzip **ru_dict** from official site 

```bash
wget http://espeak.sourceforge.net/data/ru_dict-48.zip

unzip ru_dict-48.zip
```

4. Find the name of cataloge of *espeak-data* (or *espeak-ng-data*) and move *ru_dict-48* there. In our case:

```bash
sudo mv ru_dict-48 /usr/lib/x86_64-linux-gnu/espeak-data/ru_dict
```

> In your case instead of *x86_64-linux-gnu* may be *i386-linux-gnu* or something else. If you're not sure use the search:

```bash
find /usr/lib/ -name "espeak-data"
```

5. Get the Raspberry Pi Speaking from the Command Line

After eSpeak has been successfully installed on the Raspberry Pi, run the following command to test eSpeak:

```bash
espeak-ng "Hello World" 2>/dev/null
```


You should hear “Hello World” from the speakers.  Your Raspberry Pi is speaking!