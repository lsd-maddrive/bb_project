import logging

logging.basicConfig(
    level=logging.DEBUG, format="%(asctime)s - %(name)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)

import math
import asyncio
import serial
import struct
from src.xbox_one import Joystick
from src.robot_logging import CsvLogger
from src.tcp import TcpLog
from datetime import datetime
from datetime import timedelta

from src.config import V_MAX, ANG_SPEED_MAX, START_BYTES, TCP_FLAG
from utils import calc_angle, calc_velocity, calc_angle_speed


def handle_package(port, csv_logger, tcp_client):
    """
    Reading of data from mcu. If everything is fine, save it to csv and send via tcp to matlab
    :param port: Serial port to read. Need to do because of async: can't see from outer space
    :param csv_logger: logger class exemplar. Need to do because of async: can't see from outer space
    :param tcp_client: tcp client class exemplar. Need to do because of async: can't see from outer space
    """
    package = port.read(56)
    if [package[i] for i in range(3)] == START_BYTES:
        if package[3] == 1:
            csv_logger.flush()
            csv_logger.new_file()
        elif package[3] == 0:
            csv_logger.log_line(package[4:])
        if tcp_client is not None:
            tcp_client.send(package[12:24])
    port.reset_input_buffer()


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
        port = serial.Serial("/dev/ttyACM0", 115200)
        logger.debug("USB is connected")
        if TCP_FLAG:
            tcp_client = TcpLog("192.168.90.101", 8081)
            tcp_client.open()
        else:
            tcp_client = None

        # Instantiate the controller
        joy = Joystick()

        # Maybe not necessary. Gamepad store old values for some reason. Maybe only 360
        # await asyncio.sleep(3)

        logger.debug("Connected to %s", joy.device.name)

        port.write(struct.pack("B", 185))

        time = datetime.now()
        while joy.leftTrigger() < 0.8:

            time = time + timedelta(milliseconds=100)

            ang_speed = calc_angle_speed(joy.rightYAxis(), ANG_SPEED_MAX)

            velocity_x, velocity_y = calc_velocity(
                joy.leftXAxis(), joy.leftYAxis(), joy.rightTrigger(), V_MAX
            )
            # Usart data transmit
            port.write(
                struct.pack(
                    "<fff", float(velocity_x), float(velocity_y), float(ang_speed)
                )
            )
            handle_package(port, csv_logger, tcp_client)

            await wait_until(time)

    except (FileNotFoundError, serial.serialutil.SerialException):
        logger.error("USB port is not correct. Connection failed!")
    except AttributeError:
        # to avoid exception in joy.close()
        joy = 0
        logger.error("Joystick is turned off. Check it, please!")
    except OSError:
        logger.error("Joystick fell asleep!")
    finally:
        csv_logger.stop()
        # Close out when done
        if joy != 0:
            joy.close()
            logger.debug("Joystick is closed successfully")
        if port != 0:
            port.close()
            logger.debug("Serial port is closed successfully")


async def log(csv_logger):
    while csv_logger.run:
        csv_logger.flush()
        await asyncio.sleep(5)


def main():
    csv_logger = CsvLogger("Logs")
    loop = asyncio.get_event_loop()
    tasks = [
        loop.create_task(robot_control(csv_logger)),
        loop.create_task(log(csv_logger)),
    ]
    loop.run_until_complete(asyncio.wait(tasks))
    loop.close()


if __name__ == "__main__":
    main()
