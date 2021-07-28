import logging
import asyncio
import time

import serial
import struct
from src.xbox_one import Gamepad, GamepadButtons, GamepadAxis, ExternalDeviceNotFound
from src.robot_logging import CsvLogger
from src.tcp import TcpLog
from datetime import datetime
from datetime import timedelta

from src.config import V_MAX, ANG_SPEED_MAX, START_BYTES, TCP_FLAG
from utils import calc_angle, calc_velocity, calc_angle_speed

logging.basicConfig(
    level=logging.DEBUG, format="%(asctime)s - %(name)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)


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


async def gamepad_loop(dev, event):
    while not event.is_set():
        dev.update_buttons()
        await asyncio.sleep(0.05)


async def robot_control(dev, csv_logger, event):
    try:
        port = None
        port = serial.Serial("/dev/ttyACM0", 115200)
        logger.debug("USB is connected")
        if TCP_FLAG:
            tcp_client = TcpLog("192.168.90.101", 8081)
            tcp_client.open()
        else:
            tcp_client = None

        logger.debug("Connected to %s", dev.device.name)

        port.write(struct.pack("B", 185))

        time = datetime.now()
        while not dev.read_button(GamepadButtons.LB.value):
            

            time = time + timedelta(milliseconds=100)

            ang_speed = calc_angle_speed(dev.get_axis_value(GamepadAxis.RIGHT_Y_AXIS.value), ANG_SPEED_MAX)

            velocity_x, velocity_y = calc_velocity(
                dev.get_axis_value(GamepadAxis.LEFT_X_AXIS.value), dev.get_axis_value(GamepadAxis.LEFT_Y_AXIS.value),
                dev.get_axis_value(GamepadAxis.RIGHT_TRIGGER.value), V_MAX
            )
            # Usart data transmit
            port.write(
                struct.pack(
                    "<fff", float(velocity_x), float(velocity_y), float(ang_speed)
                )
            )
            handle_package(port, csv_logger, tcp_client)

            await wait_until(time)
        event.set()

    except (FileNotFoundError, serial.serialutil.SerialException):
        logger.error("USB port is not correct. Connection failed!")
    except AttributeError:
        # to avoid exception in joy.close()
        dev = None
        logger.error("Gamepad is turned off. Check it, please!")
    except OSError:
        logger.error("Gamepad fell asleep!")
    finally:
        csv_logger.stop()
        # Close out when done
        if dev is not None:
            dev.close()
            logger.debug("Gamepad is closed successfully")
        if port is not None:
            port.close()
            logger.debug("Serial port is closed successfully")
        event.set()


async def log(csv_logger, event):
    while not event.is_set():
        csv_logger.flush()
        await asyncio.sleep(5)


def main():
    dev = None
    for _ in range(5):
        try:
            dev = Gamepad()
        except ExternalDeviceNotFound:
            logger.debug("Please check the gamepad")
            time.sleep(3)
        else:
            break
    if dev is None:
        logger.error("Gamepad is unavailable")
        return
    csv_logger = CsvLogger("Logs")
    event = asyncio.Event()
    loop = asyncio.get_event_loop()
    tasks = [
        loop.create_task(robot_control(dev, csv_logger, event)),
        loop.create_task(log(csv_logger, event)),
        loop.create_task(gamepad_loop(dev, event)),
    ]
    loop.run_until_complete(asyncio.wait(tasks))
    loop.close()


if __name__ == "__main__":
    main()
