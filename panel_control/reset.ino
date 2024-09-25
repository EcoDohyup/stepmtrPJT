#define STEP_DIR_PIN 12
#define STEP_STP_PIN 14
#define STEP_ENA_PIN 13

#define STEP_DELAY_MICROS 500 // Default delay between steps

// Variables to store motor state
bool motorRunning = false; // 모터 운행상태 표시 변수
int motorDirection = HIGH; // 기본 방향 우향으로 설정

void setup() {
  Serial.begin(115200);
  Serial.println("Motor Position Reset Control");

  // GPIO 초기화
  pinMode(STEP_ENA_PIN, OUTPUT);
  pinMode(STEP_DIR_PIN, OUTPUT);
  pinMode(STEP_STP_PIN, OUTPUT);

  // 모터 초기 무력화
  digitalWrite(STEP_ENA_PIN, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // 줄바꿈 및 공백 제거
    
    if (command == "s") {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 정지
      Serial.println("Motor stopped");
    } 
    else if (command == "d") {
      motorRunning = true;
      motorDirection = HIGH; // 모터 우향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
      Serial.println("Motor moving right");
    } 
    else if (command == "a") {
      motorRunning = true;
      motorDirection = LOW; // 모터 좌향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
      Serial.println("Motor moving left");
    }
  }

  if (motorRunning) {
    digitalWrite(STEP_DIR_PIN, motorDirection); // 방향 조절
    digitalWrite(STEP_STP_PIN, HIGH); // 스텝 신호 전송
    delayMicroseconds(STEP_DELAY_MICROS); // 펄스 사이의 시간 조정
    digitalWrite(STEP_STP_PIN, LOW);
  }
}
