using namespace std;
#define STEP_DIR_PIN 12
#define STEP_STP_PIN 14
#define STEP_ENA_PIN 13
#define LIMIT_RIGHT 4 // 우극한 리미터 핀
#define LIMIT_LEFT 2 // 좌극한 리미터 핀
#define DEFAULT_STEP_DELAY_MICROS 500 // 기본 속도

// Variables to store motor state
bool motorRunning = false; // 모터 운행상태 표시 변수
int motorDirection = HIGH; // 기본 방향 우향으로 설정
int stepDelayMicros = DEFAULT_STEP_DELAY_MICROS; // 모터 속도

void setup() {
  Serial.begin(115200);  // Initialize serial communication

  // GPIO 초기화
  pinMode(STEP_ENA_PIN, OUTPUT);
  pinMode(STEP_DIR_PIN, OUTPUT);
  pinMode(STEP_STP_PIN, OUTPUT);
  pinMode(LIMIT_RIGHT, INPUT);
  pinMode(LIMIT_LEFT, INPUT);

  Serial.println("GPIO 초기화 완료");

  digitalWrite(STEP_ENA_PIN, HIGH);  // 모터 정지 상태로 시작
  digitalWrite(STEP_DIR_PIN, motorDirection); // 기본 우향 세팅

  Serial.println("모터 초기화 완료");
}

void loop() {
  // 센서 값 읽기
  int limit_r = digitalRead(LIMIT_RIGHT);
  int limit_l = digitalRead(LIMIT_LEFT);

  // 시리얼 명령 처리
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    // 모터 정지
    if (command == "s") {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 정지
      Serial.println("모터 정지");
    } 
    // 우향으로 이동
    else if (command == "d" && limit_r == HIGH) {
      motorRunning = true;
      motorDirection = HIGH; // 모터 우향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
      Serial.println("패널 개방중");
    } 
    // 좌향으로 이동
    else if (command == "a" && limit_l == HIGH) {
      motorRunning = true;
      motorDirection = LOW; // 모터 좌향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
      Serial.println("패널 닫는중");
    }
    // 속도 조절
    else if (command == "w" && !motorRunning) {
      // 현재 속도 표시
      Serial.print("현재 속도 (마이크로초): ");
      Serial.println(stepDelayMicros);
      
      // 사용자에게 새 속도 입력 요구
      Serial.println("새 속도를 입력하세요 (마이크로초 단위): ");
      
      // 새로운 속도 값 대기
      while (Serial.available() == 0) {
        // 시리얼 입력 대기
      }

      // 새로운 속도 값 읽기
      String newSpeedString = Serial.readStringUntil('\n');
      int newSpeed = newSpeedString.toInt();  // 문자열을 정수로 변환

      // 유효한 속도 값인지 확인하고 설정
      if (newSpeed > 0) {
        stepDelayMicros = newSpeed;
        Serial.print("새 속도 설정됨: ");
        Serial.println(stepDelayMicros);
      } else {
        Serial.println("유효하지 않은 속도 값입니다.");
      }
    }
  }

  // 모터가 동작 중일 때
  if (motorRunning) {
    digitalWrite(STEP_DIR_PIN, motorDirection); // 방향 조절
    digitalWrite(STEP_STP_PIN, HIGH); // 분주비 제어
    delayMicroseconds(stepDelayMicros); // 설정된 속도로 딜레이
    digitalWrite(STEP_STP_PIN, LOW); 

    // 좌측 리미트 스위치에 도달하면 모터 정지
    if (limit_l == LOW && motorDirection == LOW) {
      motorRunning = false; 
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 완전 정지
      Serial.println("패널 닫기 완료");
    } 
    // 우측 리미트 스위치에 도달하면 모터 정지
    else if (limit_r == LOW && motorDirection == HIGH) {
      motorRunning = false; 
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 완전 정지
      Serial.println("패널 개방 완료");
    }
  }
}
