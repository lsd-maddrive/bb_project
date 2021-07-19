## Samba (local folder)

Install samba and edit .conf file

>sudo apt-get install samba samba-common-bin
>
>sudo nano /etc/samba/smb.conf

change file by altering/adding:

>workgroup = WORKGROUP
>
>wins support = yes
>
>[PiShare]
>
>comment=Raspberry Pi Share
>
>path=/home/pi/share
>
>browseable=Yes
>
>writeable=Yes
>
>only guest=no
>
>create mask=0777
>
>directory mask=0777
>
>public=no

create password

>sudo smbpasswd -a pi

Create *share* folder and start service

>mkdir ~/share
>
>sudo service smbd restart

## Change password

>passwd

follow instructions. If need to set weak password (less then 6 symbol length):

>sudo -su
>
>passwd pi
>
>exit

## turn on VNC

>sudo apt update
>
>sudo apt install realvnc-vnc-server realvnc-vnc-viewer
>
>sudo raspi-config

Go to interfacing options -> VNC -> set yes

>ifconfig

Remember ip adress! You will need it to connect via any VNC viewer from other device


## wi-fi setup

Edit interfaces file

>sudo nano /etc/network/interfaces

Edit file by adding next lines

>auto lo
>
>iface lo inet loopback
>
>iface eth0 inet manual
>
>auto wlan0
>
>allow-hotplug wlan0
>
>iface wlan0 inet manual
>
>wpa-roam /etc/wpa_supplicant/wpa_supplicant.conf

Edit wpa_supplicant file

>sudo nano /etc/wpa_supplicant/wpa_supplicant.conf

File should have 2 lines: ctrl_interface... and update_config=1
Add Router settings. No extra spaces!

>network={
>
>ssid="NETWORK NAME"
>
>psk="NETWORK PASS"
>
>}

Reboot and check if it works. If no try next:
>sudo ip link set wlan0 down
>
>sudo ip link set wlan0 up
>
>
>sudo wpa_supplicant -B -iwlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf -Dnl80211,wext
>
>sudo dhclient wlan0

## Set static ip

Edit dgcpcd file

sudo nano /etc/dhcpcd.conf

Type this at the end of file:

>nodhcp
>
>interface wlan0
>
>static ip_address=DISIRED IP/24
>
>static routers=ROUTER IP
>
>static domain_name_servers=USUALLY SAME AS ROUTER IP

## Voice sinthesis

Install espeak-ng module

>sudo apt-get install espeak-ng

You can check if it works by calling

>espeak-ng test

Make shure you have audio device to hear. Check if audio output is set to analog out, not hdmi

## Xbox gamepad driver

Install some modules

>sudo apt-get curl
>
>sudo apt-get cabextract
>
>sudo apt-get libusb-1.0-0-dev
>
>sudo apt-get systemd

Download and install xow driver

>git clone https://github.com/medusalix/xow
>
>cd xow
>
>make BUILD=RELEASE
>
>sudo make install
>
>sudo systemctl enable xow
>
>sudo systemctl start xow

## Prepare for bb

Clone bb repository

>git clone https://github.com/serykhelena/bb-8_project.git
>
>cd /bb-8_project/python
>
>pip3 install virtualenv
>
>python3 -m venv venv
>
>source venv/bin/activate
>
>pip3 install -r requirements.txt

# Enjoy!
