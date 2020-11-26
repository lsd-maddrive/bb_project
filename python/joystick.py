from datetime import datetime, timedelta
import logging
import math
import os
import sys 

import asyncio
import serial
import struct


from robot_control.xbox_one import Joystick
from robot_control.robot_logging import CsvLogger
import robot_control.tools as tools
#sys.path.append('../')
#from speaker.speaker import Speaker


logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


async def wait_until(dt):
    """
    Wait until desired datetime. If the datetime is in the past, return immediately.
    :param dt: datetime to wait until
    """
    now = datetime.now()
    delta = (dt - now).total_seconds()
    if delta > 0:
        await asyncio.sleep(delta)


async def robot_control(csv_logger):
    try:
        joy = 0
        port = 0
        port = serial.Serial('/dev/ttyACM0', 115200)
        logger.debug(f"USB is connected successfully!")
        
        # Instantiate the controller
        joy = Joystick()
        #bbSpeaker = Speaker()

        logger.debug("Connected to %s", joy.device.name)
        time = datetime.now()
        while joy.left_trigger() < 0.8:

            joy.update_buttons()

            if joy.x_button():
                #Speaker.say("Hello there! General Kenoby")
                os.popen(f'sudo espeak-ng -v "en-us" -a 100 -s 150 "Hello there! General Kenoby"')

            ang_speed = tools.calc_angle_speed(joy.right_y_axis())

            velocity_x, velocity_y = tools.calc_velocity(joy.left_x_axis(),
                                                   joy.left_y_axis(),
                                                   joy.right_trigger())
            # Usart data transmit
            port.write(struct.pack('<fff', float(velocity_x), float(velocity_y), float(ang_speed)))
            tools.read_line(port, csv_logger)

            time = time + timedelta(milliseconds=100)
            await wait_until(time)

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
    csv_logger = CsvLogger('Logs')
    loop = asyncio.get_event_loop()
    tasks = [loop.create_task(robot_control(csv_logger)), loop.create_task(log(csv_logger))]
    loop.run_until_complete(asyncio.wait(tasks))
    loop.close()


if __name__ == "__main__":
    main()
