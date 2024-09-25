import sqlite3

# 데이터베이스 연결
conn = sqlite3.connect('motor_positions.db')
cursor = conn.cursor()

# 가장 최신 10개의 motorPosition 데이터 조회
cursor.execute("SELECT * FROM motor_positions ORDER BY timestamp DESC LIMIT 10")
rows = cursor.fetchall()

# 조회된 데이터 출력
for row in rows:
    print(f"Timestamp: {row[0]}, Motor Position: {row[1]}")

# 데이터베이스 연결 종료
conn.close()
