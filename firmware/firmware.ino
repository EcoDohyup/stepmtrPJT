#define STEP_DIR_PIN 12
#define STEP_STP_PIN 14
#define STEP_ENA_PIN 13

#define STEP_DELAY_MICROS 500 // Default delay between steps

// Variables to store motor state
bool motorRunning = false; // 모터 운행상태 표시 변수
int motorDirection = HIGH; // 기본 방향 우향으로 설정


int motorPosition = 0; // 분주비, 사실상 위치값

unsigned long lastPositionSendTime = 0; // 
const unsigned long positionSendInterval = 1000; // Interval in milliseconds

void setup() {
  Serial.begin(115200);
  Serial.println("Stepper Motor Control");

  // GPIO 초기화
  pinMode(STEP_ENA_PIN, OUTPUT);
  pinMode(STEP_DIR_PIN, OUTPUT);
  pinMode(STEP_STP_PIN, OUTPUT);

  // 모터 초기 무력화
  digitalWrite(STEP_ENA_PIN, HIGH);

  Serial.println("Please send the initial position followed by a newline:");

  // 비상정지 후 재가동시 마지막 위치값 입력
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) { // 입력 10초 기다림
    if (Serial.available() > 0) {
      String initPosStr = Serial.readStringUntil('\n');
      initPosStr.trim(); // 
      motorPosition = initPosStr.toInt(); // 입력값 정수로 변환

      // 입력 성공시 표시되는 부분
      if (motorPosition != 0 || initPosStr == "0") {
        Serial.print("Initial Position Set to: ");
        Serial.println(motorPosition);
        return; // 성공시 setup 함수 종료
      } else {
        Serial.println("Invalid input. Please enter a valid integer.");
        Serial.println("Please send the initial position followed by a newline:");
      }
    }
  }

  // Timeout handling
  Serial.println("No valid input received. Using default position 0.");
  motorPosition = 0; // Default position if no valid input is received

}

void loop() {
  unsigned long currentMillis = millis();

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "s") {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 정지
    } 
    else if (command == "d") {
      motorRunning = true;
      motorDirection = HIGH; // 모터 우향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
    } 
    else if (command == "a") {
      motorRunning = true;
      motorDirection = LOW; // 모터 좌향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
    }
    /*
    else if (command == "p") {
      // 현재 펄스(위치) 시리얼모니터에 표시
      Serial.println(motorPosition);
    }
    */
  }

  if (motorRunning) {
    digitalWrite(STEP_DIR_PIN, motorDirection); // 방향 조절
    digitalWrite(STEP_STP_PIN, HIGH); // 분주비 제어
    delayMicroseconds(STEP_DELAY_MICROS); // 펄스간 시간 조정, 고정됨
    digitalWrite(STEP_STP_PIN, LOW); 
    
    // 좌우향 끝부분 닿았을때 알아서 멈추게
    if(motorPosition == 8000 && motorDirection == LOW){ // 좌한 정지
      motorRunning = false; // motorPosition 갱신 정지용
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 완전 정지
    } 
    else if(motorPosition == 152000 && motorDirection == HIGH){ // 우한 정지
      motorRunning = false; 
      digitalWrite(STEP_ENA_PIN, HIGH);
    } 
    
    if(motorDirection == HIGH){
      ++motorPosition; // 우향일때 분주비 증가
    }
    else if(motorDirection == LOW){
      --motorPosition; // 좌향일때 분주비 감소
    }
  }

  if (currentMillis - lastPositionSendTime >= positionSendInterval) {
    lastPositionSendTime = currentMillis;
    Serial.println(motorPosition);
  }
}
