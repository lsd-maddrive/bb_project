import sys
sys.path.append("../")
from src.robot_logging import CsvLogger
import asyncio
import serial
import struct


async def log():
    while csv_logger.run:
        csv_logger.flush()
        print('Flushed')
        await asyncio.sleep(5)


async def robot_read():

    port = serial.Serial('COM6', 115200)
    while True:
        a = port.read(17)
        print(struct.unpack('<bffff', a))
        csv_logger.log_line(a)
        port.write(struct.pack('<fff', 1, 1, 1))

        await asyncio.sleep(1)


csv_logger = CsvLogger('../Logs')
loop = asyncio.get_event_loop()

tasks = [loop.create_task(robot_read()), loop.create_task(log())]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
