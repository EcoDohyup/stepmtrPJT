import serial
import time

# Replace '/dev/ttyUSB0' with your serial port
try:
    ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=1)
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

def send_command(command):
    try:
        ser.write((command + '\n').encode())
        time.sleep(0.1)  # Give some time for the command to be processed
        response = ser.readline().decode().strip()
        print(response)
    except serial.SerialException as e:
        print(f"Error communicating with serial port: {e}")

initial_position = input("Enter the current motor position: ")
send_command(initial_position)

try:
    while True:
        cmd = input("Enter command (a : left, d : right, s : stop, p : current position): ")
        send_command(cmd)
except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
