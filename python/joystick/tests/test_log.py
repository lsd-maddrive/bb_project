import sys

sys.path.append("../")
from src.robot_logging import CsvLogger
import asyncio
import serial
import struct


start_bytes = [150, 139, 57]


async def log():
    while csv_logger.run:
        csv_logger.flush()
        print("Flushed")
        await asyncio.sleep(5)


def read_line(port):
    package = port.read(20)
    if [package[i] for i in range(3)] == start_bytes:
        if package[3] == 1:
            csv_logger.flush()
            csv_logger.new_file()
        elif package[3] == 0:
            csv_logger.log_line(package[4:])
    port.reset_input_buffer()


async def robot_read():
    port = serial.Serial("/dev/ttyACM0", 115200)
    print(port.name)
    count = 0
    while True:
        read_line(port)
        port.write(struct.pack("<fff", count, count * 2, count * 3))
        count = count + 1

        await asyncio.sleep(0.1)


csv_logger = CsvLogger("../Logs")
loop = asyncio.get_event_loop()

tasks = [loop.create_task(robot_read()), loop.create_task(log())]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
