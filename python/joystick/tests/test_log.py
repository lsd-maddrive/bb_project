import sys
sys.path.append("../")
from src.robot_logging import CsvLogger
import asyncio
import serial
import struct


async def log():
    while csv_logger.run:
        csv_logger.flush()
        await asyncio.sleep(5)


async def robot_read():

    port = serial.Serial('COM3', 115200)
    print(port)
    while True:
        a = port.read(17)
        print('hi')
        print(a)
        csv_logger.log_line(a)
        port.write(struct.pack('<fff', 1, 1, 1))
        await asyncio.sleep(0.1)


csv_logger = CsvLogger('/Logs')
loop = asyncio.get_event_loop()

tasks = [loop.create_task(robot_read()), loop.create_task(log())]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
