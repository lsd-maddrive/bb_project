import math
from typing import Tuple


def calc_angle(x: float, y: float) -> float:
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


def calc_velocity(x: float, y: float, velocity: float, v_max: float) -> Tuple[float, float]:
    """
    Calculation of axial velocities based on two axial values and vector velocity
    :param x:   x axis value [-1 1]
    :param y:   y axis value [-1 1]
    :param velocity:    normalized linear velocity [0 1]
    :param v_max: velocity maximum value, m/s
    :return:    axial velocities [-V_MAX V_MAX], m/s
    """
    if math.sqrt(x ** 2 + y ** 2) < 0.15:
        x, y = 0.0, 0.0
    velocity = velocity * v_max
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


def calc_angle_speed(omega: float, ang_speed_max: float) -> float:
    """
    Calculation of angular speed
    :param omega: normalized angular speed [-1 1]
    :param ang_speed_max: angular speed maximum value, deg/s
    :return: Angular speed [0 360] grad/s
    """
    if abs(omega) < 0.15:
        return 0.0
    return omega * ang_speed_max
