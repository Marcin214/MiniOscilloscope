import serial
import time
import test

serial_port = serial.Serial(port='COM3',baudrate =921600, timeout = 1)
serial_port.flushInput()

data_length = 2000
data        = []
data_sums   = []
time_stamps = []

transaction_trigger = bytearray(b'\xff')
transaction_number  = 10000

time_start = time.clock()
for i in range(transaction_number):
    serial_port.write(transaction_trigger)
    data = serial_port.read(data_length)
    data_sums.append(sum(data))
    time_stamps.append(time.clock() - time_start)

if test.check_corretness_mod256(data_sums, data_length):
    test.generate_report(time_start, time_stamps)
else:
    print("test failed")
