import serial
import time
import sys
import select
import sqlite3

# SQLite 데이터베이스 연결 및 테이블 생성
conn = sqlite3.connect('motor_positions.db')
cursor = conn.cursor()

# 테이블이 존재하지 않으면 생성
cursor.execute('''CREATE TABLE IF NOT EXISTS motor_positions (
                    timestamp TEXT,
                    motor_position INTEGER
                )''')
conn.commit()

def store_position(position):
    # motorPosition과 현재 시간을 DB에 저장
    cursor.execute("INSERT INTO motor_positions (timestamp, motor_position) VALUES (datetime('now'), ?)", (position,))
    conn.commit()

def get_latest_position():
    # 가장 최신의 motorPosition 가져오기
    cursor.execute("SELECT motor_position FROM motor_positions ORDER BY timestamp DESC LIMIT 1")
    result = cursor.fetchone()
    return result[0] if result else None

# 마지막 위치를 데이터베이스에서 가져오기
latest_position = get_latest_position()

# 사용자가 마지막 위치를 사용할지 여부를 선택하도록 안내
if latest_position is not None:
    choice = input(f"마지막 박스 위치는 {latest_position} 입니다. 해당 값으로 세팅하시겠습니까? (y/x): ").strip().lower()
    if choice == 'y':
        initial_position = str(latest_position)  # 이 부분에서 정수를 문자열로 변환
    elif choice == 'x':
        initial_position = input("모터의 현재 위치를 입력하세요. : ").strip()
else:
    initial_position = input("모터의 현재 위치를 입력하세요. : ").strip()

# 시리얼 포트 초기화 및 명령 전송
try:
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
except serial.SerialException as e:
    print(f"시리얼 포트를 여는 중 오류 발생: {e}")
    exit()

def send_command(command):
    try:
        ser.write((command + '\n').encode())
        time.sleep(0.1)  # 명령이 처리될 시간을 잠시 대기
        response = ser.readline().decode().strip()
        if response:  # 응답이 있는 경우만 출력
            print(response)
    except serial.SerialException as e:
        print(f"시리얼 포트 통신 중 오류 발생: {e}")

# 초기 위치 전송
send_command(initial_position)
print("명령을 입력하세요 (a : left, d : right, s : stop): ")

try:
    while True:
        # 명령어 입력 대기
        input_ready, _, _ = select.select([sys.stdin], [], [], 0.1)
        
        if input_ready:
            cmd = sys.stdin.readline().strip()
            send_command(cmd)
        
        # 시리얼 데이터 읽기 및 출력
        if ser.in_waiting > 0:
            response = ser.readline().decode().strip()
            if response.isdigit():  # 숫자인 경우에만 motorPosition으로 간주
                motor_position = response
                #print(motor_position)
                store_position(motor_position)  # 데이터베이스에 저장
except KeyboardInterrupt:
    print("종료 중...")
finally:
    ser.close()
    conn.close()  # 데이터베이스 연결 종료
