import struct
import csv
import datetime
import os.path


class CsvLogger:
    def __init__(self, path):
        self.folder = path
        self.buff = []
        self.current_file = self.folder + '/log_{date}.csv'.format(date=datetime.datetime.today().strftime('%m_%d_%H_%M_%S'))
        self.header = ['time', 'par1', 'par2', 'par3', 'par4', 'par5']
        self.run = 1

    def flush(self):
        if not self.buff:
            return
        if not os.path.exists(self.current_file):
            self.new_file()
        with open(self.current_file, 'a', newline='') as log_file:
            csv.writer(log_file).writerows(self.buff)
        self.buff = []

    def log_line(self, raw_data):
        package = struct.unpack('<bffff', raw_data)
        if package[0] == 1:
            self.flush()
            self.new_file()
            return
        string_to_csv = [datetime.datetime.today().strftime('%H.%M.%S.%f')[:-3]] + ['{:.3f}'.format(i) for i in package[1:]]
        self.buff.append(string_to_csv)

    def new_file(self):
        self.current_file = self.folder + '/log_{date}.csv'.format(date=datetime.datetime.today().strftime('%m_%d_%H_%M_%S'))
        print(self.current_file)
        with open(self.current_file, 'w', newline='') as log_file:
            csv.writer(log_file).writerow(self.header)

    def stop(self):
        self.flush()
        self.run = 0
