import struct
import csv
import datetime
import os.path

from utils.fs import get_date_string


class CsvLogger:
    def __init__(self, path: str):
        self.folder = path
        os.makedirs(self.folder, exist_ok=True)

        self.buff = []

        self.current_file = os.path.join(self.folder, f"log_{get_date_string()}")
        if not os.path.exists(self.current_file):
            self.new_file()

        # fmt: off
        self.header = [
            "time", "stm_time", "SetZAngle",
            "XAngle", "YAngle", "ZAngle",
            "VxSet", "VySet", "AngularVelocitySet",
            "VxLocal", "VyLocal",
            "wA", "wB", "wC",
        ]
        # fmt: on

    def flush(self):
        if not self.buff:
            return

        with open(self.current_file, "a", newline="") as log_file:
            csv.writer(log_file).writerows(self.buff)
        self.buff = []

    def log_line(self, raw_data):
        package = struct.unpack("<fffffffffffff", raw_data)
        # TODO: replace datetime usage with function
        string_to_csv = [datetime.datetime.today().strftime("%H:%M:%S.%f")[:-3]] + [
            "{:.3f}".format(i) for i in package
        ]
        self.buff.append(string_to_csv)

    def new_file(self):
        with open(self.current_file, "w", newline="") as log_file:
            csv.writer(log_file).writerow(self.header)
