import logging
import os

import asyncio
import serial
import struct

from bb_project.csv_logging import CsvLogger

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.abspath(os.path.join(CURRENT_DIR, os.pardir))
LOG_DIR = os.path.join(PROJECT_DIR, "logs")

logging.basicConfig(
    level=logging.DEBUG, format="%(asctime)s - %(name)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)


async def log(csv_logger):
    while csv_logger.run:
        csv_logger.flush()
        logger.info("Flushed")
        await asyncio.sleep(5)


def read_line(port, csv_logger):
    start_bytes = [150, 139, 57]
    package = port.read(20)
    if [package[i] for i in range(3)] == start_bytes:
        if package[3] == 1:
            csv_logger.flush()
            csv_logger.new_file()
        elif package[3] == 0:
            csv_logger.log_line(package[4:])
    port.reset_input_buffer()


async def robot_read(csv_logger):
    port = serial.Serial("/dev/ttyACM0", 115200)
    logger.info(f"Port: {port.name}")
    count = 0
    while True:
        read_line(port, csv_logger)
        port.write(struct.pack("<fff", count, count * 2, count * 3))
        count = count + 1

        await asyncio.sleep(0.1)


def main():
    os.makedirs(LOG_DIR, exist_ok=True)
    csv_logger = CsvLogger(LOG_DIR)
    loop = asyncio.get_event_loop()

    tasks = [
        loop.create_task(robot_read(csv_logger)),
        loop.create_task(log(csv_logger)),
    ]
    loop.run_until_complete(asyncio.wait(tasks))
    loop.close()


if __name__ == "__main__":
    main()
