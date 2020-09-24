import struct
import serial
import csv



data = serial.read(20)
raw_log = [str(i) for i in struct.unpack('<fffff', data)]
string_to_csv = ','join(raw_log)
with open(log.csv, 'w', newline='') as csv_file:
    csv.writer.writerow(string_to_csv)
