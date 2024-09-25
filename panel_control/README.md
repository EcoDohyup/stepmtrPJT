# 태양광 패널 스테퍼 모터 제어 시스템

## 초기 세팅
**보드에 펌웨어 이미 업로드 되어 있다고 가정**
1. 시리얼 연결 및 ttyUSB 포트 파악
2. main.py에 ttyUSB* 수정
3. python3 main.py 로 먼저 실행
4. 보드 리셋 후 모터 초기 값 입력 - 보통 모터쪽은 7999, 모터와 먼 쪽은 152001로 입력하면 됨
5. 이후 터미널에 a로 좌향, d로 우향 입력, 일시정지시 s 입력

## 첨부 코드
### main.py
- 모터 명령 전송하는 코드

### firmware.ino
- ESP32 보드에 올라갈 펌웨어, 핀 세팅 아래 참고

### reset.ino
- 위치 값 틀어졌을때 모터 양 끝단으로 옮기기 위한 비상 펌웨어
- a, d, s로 조절하고, 모터 끝으로 옮기면 역할 끝
- 이후 모터 위치 보정은 원래 펌웨어로 조절

## 하드웨어 결선
### 전체 결선
![전체 결선](panel_control/environment.excalidraw)

### 드라이버-보드
![드라이버-보드](panel_control/driverpin.excalidraw)

### 드라이버-모터
![드라이버-모터](panel_control/motor-driver.excalidraw)
