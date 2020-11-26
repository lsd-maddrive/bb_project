from robot_control.config import V_MAX, ANG_SPEED_MAX, START_BYTES
import math


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
            angle = angle + math.pi
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


def read_line(port, csv_logger):
    """
    Reading of data from mcu. If everything is fine, saving it to csv
    :param port: Serial port to read. Need to do because of async: can't see from outer space
    :param csv_logger: logger class exemplar. Need to do because of async: can't see from outer space
    """
    package = port.read(48)
    if [package[i] for i in range(3)] == START_BYTES:
        if package[3] == 1:
            csv_logger.flush()
            csv_logger.new_file()
        elif package[3] == 0:
            csv_logger.log_line(package[4:])
    port.reset_input_buffer()