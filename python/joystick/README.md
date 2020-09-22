# How to deal with Xbox joystick 


1. Check available software

```bash
sudo apt-get update
sudo apt-get upgrade
```

2. Install xow driver 

* 2.1. Install libusb

```bash
sudo apt-get install libusb-1.0-0-dev
```

* 2.2. Install cabextract

```bash
sudo apt-get install cabextract
```

* 2.3. Follow the original [README](https://github.com/medusalix/xow)


3. Install all required modules

```bash
pip3 install -r requirements.txt
```

4. Run python-script 

```bash
python3 joystick.py 
```
