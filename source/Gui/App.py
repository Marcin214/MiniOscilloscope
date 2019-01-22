from Gui import *
import serial

SAMPLES_NR=1000
CHANNEL_NR=2

serial_port = serial.Serial(port = 'COM3', write_timeout=0.1, inter_byte_timeout=0.1)
serial_port.flushInput()
RTS= bytearray(b'\xff')

gui=GUI(CHANNEL_NR,SAMPLES_NR)
paused = False

while True:

    key  = gui.get_key()
    if key == ' ':
        paused = not paused
        gui.toggle_state()
    elif key  == 'escape':
        break

    if not paused:
        try:
            serial_port.flushInput()
            serial_port.write(RTS)
            for channel in range(CHANNEL_NR):
                buffer = serial_port.read(SAMPLES_NR)
                gui.set_y(channel, buffer)
        except:
            pass

    gui.plot()
    gui.pause()