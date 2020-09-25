import asyncio
import struct
import csv
import random
import datetime


class CsvLogger:
    def __init__(self):
        self.buff = []
        self.current_file = 'log.csv'
        self.header = ['time', 'par1', 'par2', 'par3', 'par4', 'par5']

    def flush(self):
        if not self.buff:
            return 0
        with open(self.current_file, 'a', newline='') as log_file:
            csv.writer(log_file).writerows(self.buff)
        self.buff = []
        return 1

    def log_line(self, raw_data):
        string_to_csv = [datetime.datetime.today().strftime('%H.%M.%S.%f')[:-3]] + ['{:.3f}'.format(i) for i in struct.unpack('<fffff', raw_data)]
        print(string_to_csv)
        if string_to_csv[1] == '-1.000':
            self.new_file()
            return
        self.buff.append(string_to_csv)

    def new_file(self):
        self.current_file = 'log_{date}.csv'.format(date=datetime.datetime.today().strftime('%H_%M_%S'))
        with open(self.current_file, 'w', newline='') as log_file:
            csv.writer(log_file).writerow(self.header)


async def main():
    test.log_line(struct.pack('<fffff', -1, 1, 2, 3, 4))
    for i in range(10):
        a = [random.random() for _ in range(5)]
        test.log_line(struct.pack('<fffff', a[0], a[1], a[2], a[3], a[4]))
        await asyncio.sleep(0.5)


async def log():
    stop = 1
    while stop == 1:
        stop = test.flush()
        await asyncio.sleep(2)

test = CsvLogger()
loop = asyncio.get_event_loop()
tasks = [loop.create_task(main()), loop.create_task(log())]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()