import serial
import sys
import select

# Replace '/dev/ttyUSB0' with your serial port
try:
    ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=1)
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

def send_command(command):
    try:
        ser.write((command + '\n').encode())
        # Remove the short delay, as it might cause issues with reading the position
    except serial.SerialException as e:
        print(f"Error communicating with serial port: {e}")

# Prompt for and send the initial position
initial_position = input("Enter initial motor position: ")
send_command(initial_position)

print("Enter command (a: left, d: right, s: stop, p: get position): ")

try:
    while True:
        input_ready, _, _ = select.select([sys.stdin], [], [], 0.1)
        
        if input_ready:
            cmd = sys.stdin.readline().strip()
            send_command(cmd)
        
        # Continuously read and print any available data from the serial port
        if ser.in_waiting > 0:  # Check if there is data in the buffer
            response = ser.readline().decode('utf-8', errors='ignore').strip()
            if response:
                print(response)
                
except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
