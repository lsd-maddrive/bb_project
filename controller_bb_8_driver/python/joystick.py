import xbox
import time
import math
import serial
import struct

port = serial.Serial('/dev/ttyUSB0', 115200)
port.open()


# Instantiate the controller
joy = xbox.Joystick()

V_max = 0.5         # Max linear velocity [m/s]
ang_speed_max = 1   # Max angular speed [grad/s]

while joy.leftTrigger() < 0.8:
    # Read joystick data
    x_axis = joy.leftX()
    y_axis = joy.leftY()
    velocity = joy.rightTrigger()
    ang_speed_raw = joy.rightX()

    # Dead zone for sticks
    if abs(ang_speed_raw) < 0.15:
        ang_speed_raw = 0
    if math.sqrt(x_axis ** 2 + y_axis ** 2) < 0.15:
        x_axis = 0
        y_axis = 0

    # Calculate direction
    if (x_axis == 0) and (y_axis == 0):
        angle = -1
    elif x_axis > 0:
        angle = math.atan(y_axis / x_axis)
        if y_axis < 0:
            angle = angle + math.pi * 2
    elif x_axis < 0:
        angle = math.atan(y_axis / x_axis) + math.pi
    elif x_axis == 0:
        if y_axis > 0:
            angle = math.pi / 2
        else:
            angle = 3 * math.pi / 2

    # To prevent uncertainty
    if angle == -1:
        velocity_x = 0
        velocity_y = 0
    # Calculate axial velocity
    else:
        velocity_x = velocity * math.cos(angle) * V_max
        velocity_y = velocity * math.sin(angle) * V_max

    # Just in case
    if abs(velocity_y) < 0.01:
        velocity_y = 0
    if abs(velocity_x) < 0.01:
        velocity_x = 0

    # Calculate angular speed set
    ang_speed = ang_speed_raw * ang_speed_max
    data_pack = struct.pack('<hhh', int(velocity_x * 1000), int(velocity_y * 1000), int(ang_speed * 1000))
    # Here should be data transmit
    port.write(data_pack)
    time.sleep(0.1)
# Close out when done
joy.close()
port.close()