import struct
import csv
import datetime
import os.path


class CsvLogger:
    def __init__(self, path):
        self.folder = path
        self.buff = []
        self.current_file = self.folder + '/log_{date}.csv'.format(date=datetime.datetime.today().strftime('%m_%d_%H_%M_%S'))
        self.header = ['time', 'SetAngle', 'CurrentAngle', 'VxSet', 'VySet', 'AngularVelocitySet', 'VxLocal', 'VyLocal', 'wA', 'wB', 'wC']
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
        package = struct.unpack('<ffffffffff', raw_data)
        string_to_csv = [datetime.datetime.today().strftime('%H:%M:%S.%f')[:-3]] + ['{:.3f}'.format(i) for i in package]
        self.buff.append(string_to_csv)

    def new_file(self):
        self.current_file = self.folder + '/log_{date}.csv'.format(date=datetime.datetime.today().strftime('%m_%d_%H_%M_%S'))
        with open(self.current_file, 'w', newline='') as log_file:
            csv.writer(log_file).writerow(self.header)

    def stop(self):
        self.flush()
        self.run = 0
