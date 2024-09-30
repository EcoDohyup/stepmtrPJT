import serial
import sys
import time
import msvcrt

# 시리얼 포트 설정 (필요에 따라 수정)
SERIAL_PORT = 'COM3'
BAUD_RATE = 115200

# 시리얼 포트 열기
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    sys.exit()

def send_command(command):
    try:
        ser.write((command + '\n').encode())
    except serial.SerialException as e:
        print(f"Error communicating with serial port: {e}")

# 초기 모터 위치 입력 및 전송
initial_position = input("Enter initial motor position: ")
send_command(initial_position)

print("Enter command (a: left, d: right, s: stop, p: get position): ")

try:
    while True:
        # 키 입력이 있는지 확인
        if msvcrt.kbhit():
            # 키를 읽고 디코딩
            cmd = msvcrt.getche().decode().strip()
            print()  # 줄 바꿈
            send_command(cmd)
        
        # 시리얼 포트에서 데이터가 있는지 확인
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8', errors='ignore').strip()
            if response:
                print(f"Received: {response}")
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("\nExiting...")
finally:
    ser.close()