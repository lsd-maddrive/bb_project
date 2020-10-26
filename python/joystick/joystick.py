import logging 
import math
import asyncio
import serial
import struct
from src.xbox_one import Joystick
from src.robot_logging import CsvLogger

from src.config import V_MAX, ANG_SPEED_MAX, START_BYTES


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
    package = port.read(20)
    if [package[i] for i in range(3)] == START_BYTES:
        if package[3] == 1:
            csv_logger.flush()
            csv_logger.new_file()
        elif package[3] == 0:
            csv_logger.log_line(package[4:])
    port.reset_input_buffer()


async def robot_control(csv_logger):
    try:
        joy = 0
        port = 0
        port = serial.Serial('/dev/ttyACM0', 115200)
        logger.debug(f"USB is connected successfully!")
        
        # Instantiate the controller
        joy = Joystick()

        # Maybe not necessary. Gamepad store old values for some reason. Maybe only 360
        await asyncio.sleep(3)

        logger.debug("Connected to %s", joy.device.name)


        while joy.leftTrigger() < 0.8:
            ang_speed = calc_angle_speed(joy.rightYAxis())

            velocity_x, velocity_y = calc_velocity(joy.leftXAxis(),
                                                   joy.leftYAxis(),
                                                   joy.rightTrigger())
            # Usart data transmit
            port.write(struct.pack('<fff', float(velocity_x), float(velocity_y), float(ang_speed)))
            read_line(port, csv_logger)
            await asyncio.sleep(0.1)

    except (FileNotFoundError, serial.serialutil.SerialException):
        logger.error(f"USB port is not correct. Connection failed!")
    except AttributeError as err:
        # to avoid exception in joy.close()
        joy = 0
        logger.error(f"Joystick is turned off. Check it, please!")
    except OSError as err: 
        logger.error(f"Joystick fell asleep!")
    finally:
        csv_logger.stop()
        # Close out when done
        if joy is not 0:
            joy.close()
            logger.debug("Joystick is closed successfully")
        if port is not 0:
            port.close()
            logger.debug("Serial port is closed successfully")


async def log(csv_logger):
    while csv_logger.run:
        csv_logger.flush()
        await asyncio.sleep(5)


def main():
    csv_logger = CsvLogger('/Logs')
    loop = asyncio.get_event_loop()
    tasks = [loop.create_task(robot_control(csv_logger)), loop.create_task(log(csv_logger))]
    loop.run_until_complete(asyncio.wait(tasks))
    loop.close()


if __name__ == "__main__":
    main()
