#define STEP_DIR_PIN 26  // 모터 방향 제어 핀
#define STEP_STP_PIN 25  // 모터 스텝 제어 핀
#define STEP_ENA_PIN 33  // 모터 활성화 핀
#define LIMIT_RIGHT 4    // 우측 리미트 스위치 핀
#define LIMIT_LEFT 15    // 좌측 리미트 스위치 핀

#define DEFAULT_STEP_DELAY_MICROS 500  // 기본 스텝 딜레이 (모터 속도)
#define DEBOUNCE_DELAY 15  // 디바운스 지연 시간 (밀리초)

volatile bool motorRunning = false;     // 모터가 동작 중인지 여부
volatile int motorDirection = HIGH;     // 모터 방향
volatile bool limitRightTriggered = false;  // 우측 리미트 스위치가 눌렸는지 여부
volatile bool limitLeftTriggered = false;   // 좌측 리미트 스위치가 눌렸는지 여부

unsigned long lastDebounceTimeRight = 0;  // 우측 리미트 스위치 디바운스 시간
unsigned long lastDebounceTimeLeft = 0;   // 좌측 리미트 스위치 디바운스 시간
int stepDelayMicros = DEFAULT_STEP_DELAY_MICROS;  // 모터 속도
int motorPosition = 0;

// 우측 리미트 스위치 인터럽트 핸들러
void IRAM_ATTR handleLimitRight() {
  unsigned long currentTime = millis();
  
  // 디바운스 처리: 지정된 시간 내에 신호가 중복되지 않도록 필터링
  if (currentTime - lastDebounceTimeRight > DEBOUNCE_DELAY) {
    lastDebounceTimeRight = currentTime;
    limitRightTriggered = (digitalRead(LIMIT_RIGHT) == LOW);  // LOW일 때 트리거됨
    
    // 모터가 우향으로 이동 중일 때 리미트 스위치가 눌리면 모터 멈춤
    if (limitRightTriggered && motorDirection == HIGH) {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH);  // 모터 비활성화
    }
  }
}

// 좌측 리미트 스위치 인터럽트 핸들러
void IRAM_ATTR handleLimitLeft() {
  unsigned long currentTime = millis();
  
  // 디바운스 처리: 지정된 시간 내에 신호가 중복되지 않도록 필터링
  if (currentTime - lastDebounceTimeLeft > DEBOUNCE_DELAY) {
    lastDebounceTimeLeft = currentTime;
    limitLeftTriggered = (digitalRead(LIMIT_LEFT) == LOW);  // LOW일 때 트리거됨
    
    // 모터가 좌향으로 이동 중일 때 리미트 스위치가 눌리면 모터 멈춤
    if (limitLeftTriggered && motorDirection == LOW) {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH);  // 모터 비활성화
    }
  }
}

void setup() {
  Serial.begin(115200);  // 시리얼 통신 초기화

  // 모터 핀 설정
  pinMode(STEP_ENA_PIN, OUTPUT);
  pinMode(STEP_DIR_PIN, OUTPUT);
  pinMode(STEP_STP_PIN, OUTPUT);

  // 리미트 스위치 핀 설정 (내부 풀업 저항 사용)
  pinMode(LIMIT_RIGHT, INPUT_PULLUP);
  pinMode(LIMIT_LEFT, INPUT_PULLUP);

  // 리미트 스위치에 인터럽트 설정 (CHANGE 모드)
  attachInterrupt(digitalPinToInterrupt(LIMIT_RIGHT), handleLimitRight, FALLING);
  attachInterrupt(digitalPinToInterrupt(LIMIT_LEFT), handleLimitLeft, FALLING);

  // 모터 기본 상태 설정 (비활성화 상태로 시작)
  digitalWrite(STEP_ENA_PIN, HIGH);  
  digitalWrite(STEP_DIR_PIN, motorDirection);  // 기본 우향 설정


  Serial.println("모터 및 리미트 스위치 초기화 완료");

  Serial.println("현재 모터 위치 입력 : ");
  unsigned long startTime = millis();
  while (millis() - startTime < 20000) { // 입력 10초 기다림
    if (Serial.available() > 0) {
      String initPosStr = Serial.readStringUntil('\n');
      initPosStr.trim(); 
      motorPosition = initPosStr.toInt(); 

      // 입력 성공시 표시되는 부분
      if (motorPosition != 0 || initPosStr == "0") {
        Serial.print("Initial Position Set to: ");
        Serial.println(motorPosition);
        return;
      } else {
        Serial.println("Invalid input. Please enter a valid integer.");
        Serial.println("Please send the initial position followed by a newline:");
      }
    }
  }

  // Timeout handling
  Serial.println("No valid input received. Using default position 0.");
  motorPosition = 0; 

}

void loop() {
  // 시리얼 명령 처리
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // 모터 정지 명령
    if (command == "s") {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH);  // 모터 비활성화
      Serial.println("모터 정지");
    }
    // 우향으로 이동 명령
    else if (command == "d" && !limitRightTriggered) {
      motorRunning = true;
      motorDirection = HIGH;  // 모터 방향 우향
      digitalWrite(STEP_ENA_PIN, LOW);  // 모터 활성화
      Serial.println("모터 우향 이동 중");
    }
    // 좌향으로 이동 명령
    else if (command == "a" && !limitLeftTriggered) {
      motorRunning = true;
      motorDirection = LOW;  // 모터 방향 좌향
      digitalWrite(STEP_ENA_PIN, LOW);  // 모터 활성화
      Serial.println("모터 좌향 이동 중");
    }
    else if (command == "p"){
      Serial.print("현재 위치 : ");
      Serial.println(motorPosition);
    }
  }


  // 모터가 동작 중일 때
  if (motorRunning) {
    digitalWrite(STEP_DIR_PIN, motorDirection);  // 모터 방향 설정
    digitalWrite(STEP_STP_PIN, HIGH);  // 스텝 신호 HIGH
    delayMicroseconds(stepDelayMicros);  // 스텝 간 딜레이
    digitalWrite(STEP_STP_PIN, LOW);  // 스텝 신호 LOW

    if(motorDirection == HIGH){
      ++motorPosition;
    }
    else if(motorDirection == LOW){
      --motorPosition;
    }

    if((motorPosition <= 400 && motorDirection == LOW) || (motorPosition >= 161200 && motorDirection == HIGH)){
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH);
    }
  }

  // 리미트 스위치 상태 확인 및 출력
  if (limitRightTriggered) {
    //Serial.println("우측 리미트 스위치가 트리거되었습니다!");
    limitRightTriggered = false;  // 한 번 출력한 후 플래그 리셋
  }
  if (limitLeftTriggered) {
    //Serial.println("좌측 리미트 스위치가 트리거되었습니다!");
    limitLeftTriggered = false;  // 한 번 출력한 후 플래그 리셋
  }
}
