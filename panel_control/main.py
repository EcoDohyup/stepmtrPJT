import serial
import sys
import select

# 시리얼 포트 맞춰 설정
try:
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit()

def send_command(command):
    try:
        ser.write((command + '\n').encode())

    except serial.SerialException as e:
        print(f"Error communicating with serial port: {e}")

# 초기 위치값 입력
print("initial position (500 or 159500)")
send_command(initial_position)

print("명령 입력 (a: panel Close, d: panel Open, s: stop, w: panel speed): ")

try:
    while True:
        input_ready, _, _ = select.select([sys.stdin], [], [], 0.1)
        
        if input_ready:
            cmd = sys.stdin.readline().strip()
            send_command(cmd)
        
        """
        # 시리얼로 숫자 입력 받음, 1초 한번 전송받으므로 정확한 실시간 값이 아님에 주의. 버퍼에 올라오면 값 입력받는 방식
        if ser.in_waiting > 0: 
            response = ser.readline().decode('utf-8', errors='ignore').strip()
            if response:
                print(response)
        """
                
except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
