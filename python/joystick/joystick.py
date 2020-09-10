import logging 
import math
import time

import serial
import struct
import xbox

from config import V_MAX, ANG_SPEED_MAX


logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


def calc_angle(x, y):
    """
    Calculation of angle based on axial projections
    :param x:   x axis value [-1 1]
    :param y:   y axis value [-1 1]
    :return:    angle [0 2Pi]
    """
    if x == 0:
        if y == 0:
            angle = -1
        elif y > 0:
            angle = math.pi / 2
        else:
            angle = 3 * math.pi / 2
    else:
        angle = math.atan(y / x)
        if x < 0:
            angle = angle + math.pi / 2
        angle = angle % (2 * math.pi)
    return angle


def calc_velocity(x, y, velocity):
    """
    Calculation of axial velocities based on two axial values and vector velocity
    :param x:   x axis value [-1 1]
    :param y:   y axis value [-1 1]
    :param velocity:    linear velocity [0 V_MAX], m/s
    :return:    axial velocities [-V_MAX V_MAX], m/s
    """
    if math.sqrt(x ** 2 + y ** 2) < 0.15:
        x, y = 0.0, 0.0
    velocity = velocity * V_MAX
    # Calculate direction
    angle = calc_angle(x, y)
    # To prevent uncertainty
    if angle == -1:
        velocity_x, velocity_y = 0.0, 0.0
    # Calculate axial velocity
    else:
        velocity_x = velocity * math.cos(angle)
        velocity_y = velocity * math.sin(angle)
    # Just in case
    if abs(velocity_y) < 0.01:
        velocity_y = 0.0
    if abs(velocity_x) < 0.01:
        velocity_x = 0.0
    return velocity_x, velocity_y


def calc_angle_speed(omega):
    """
    Calculation of angular speed
    :param omega: input value [-1 1]
    :return: Angular speed [0 360] grad/s
    """
    if abs(omega) < 0.15:
        return 0.0
    return omega * ANG_SPEED_MAX


def main():
    try:
        joy = 0
        port = 0
        port = serial.Serial('/dev/ttyUSB0', 115200)
        logger.debug(f"USB is connected successfully!")

        # Instantiate the controller
        joy = xbox.Joystick()

        while joy.leftTrigger() < 0.8:
            ang_speed = calc_angle_speed(joy.rightX())
            # Joystick we use has reversed axis, so X and Y axis are switched
            velocity_x, velocity_y = calc_velocity(joy.leftY(), joy.leftX(), joy.rightTrigger())
            # Usart data transmit
            port.write(struct.pack('<fff', float(velocity_x), float(velocity_y), float(ang_speed)))
            time.sleep(0.1)

    except (FileNotFoundError, serial.serialutil.SerialException):
        logger.error(f"USB port is not correct. Connection failed!")
        # return
    finally:
        # Close out when done
        if joy is not 0:
            joy.close()
            logger.debug("Joystick is closed successfully")
        if port is not 0:
            port.close()
            logger.debug("Serial port is closed successfully")


if __name__ == "__main__":
    main()
