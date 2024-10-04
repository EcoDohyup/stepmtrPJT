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
int motorPosition = 0;  // 모터 현재 위치 변수

// 모터 위치의 인터럽트 범위 제한
const int INTERVAL_LOW = 1000;
const int INTERVAL_HIGH = 158000;

// 우측 리미트 스위치 인터럽트 핸들러
void IRAM_ATTR handleLimitRight() {
  unsigned long currentTime = millis();
  
  // 디바운스 처리
  if (currentTime - lastDebounceTimeRight > DEBOUNCE_DELAY) {
    lastDebounceTimeRight = currentTime;

    // FALLING 트리거 발생 후 실제로 신호가 LOW 상태인지 확인 (유효한 눌림인지 확인)
    if (digitalRead(LIMIT_RIGHT) == LOW) {
      delay(DEBOUNCE_DELAY);  // 디바운스 시간만큼 기다림
      
      // 다시 확인: 여전히 LOW 상태라면 유효한 신호로 간주
      if (digitalRead(LIMIT_RIGHT) == LOW) {
        limitRightTriggered = true;  // 우측 리미트 스위치 트리거
        if (motorDirection == HIGH) {
          motorRunning = false;  // 모터 정지
          digitalWrite(STEP_ENA_PIN, HIGH);  // 모터 비활성화
          Serial.println("우측 리미트 스위치 트리거됨 (모터 정지)");
        }
      }
    }
  }
}

// 좌측 리미트 스위치 인터럽트 핸들러
void IRAM_ATTR handleLimitLeft() {
  unsigned long currentTime = millis();
  
  // 디바운스 처리
  if (currentTime - lastDebounceTimeLeft > DEBOUNCE_DELAY) {
    lastDebounceTimeLeft = currentTime;

    // FALLING 트리거 발생 후 실제로 신호가 LOW 상태인지 확인 (유효한 눌림인지 확인)
    if (digitalRead(LIMIT_LEFT) == LOW) {
      delay(DEBOUNCE_DELAY);  // 디바운스 시간만큼 기다림
      
      // 다시 확인: 여전히 LOW 상태라면 유효한 신호로 간주
      if (digitalRead(LIMIT_LEFT) == LOW) {
        limitLeftTriggered = true;  // 좌측 리미트 스위치 트리거
        if (motorDirection == LOW) {
          motorRunning = false;  // 모터 정지
          digitalWrite(STEP_ENA_PIN, HIGH);  // 모터 비활성화
          Serial.println("좌측 리미트 스위치 트리거됨 (모터 정지)");
        }
      }
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

  // 리미트 스위치에 인터럽트 설정 (FALLING 모드)
  attachInterrupt(digitalPinToInterrupt(LIMIT_RIGHT), handleLimitRight, FALLING);
  attachInterrupt(digitalPinToInterrupt(LIMIT_LEFT), handleLimitLeft, FALLING);

  // 모터 기본 상태 설정 (비활성화 상태로 시작)
  digitalWrite(STEP_ENA_PIN, HIGH);  
  digitalWrite(STEP_DIR_PIN, motorDirection);  // 기본 우향 설정

  Serial.println("모터 및 리미트 스위치 초기화 완료");

  // 현재 모터 위치 입력 받기
  Serial.println("현재 모터 위치 입력, 닫혀있을 때 : c, 열려있을 때 : o");
  unsigned long startTime = millis();
  while (millis() - startTime < 20000) { // 입력 대기 시간 20초
    if (Serial.available() > 0) {
      String initPosStr = Serial.readStringUntil('\n');
      initPosStr.trim(); 
      if(initPosStr == "c"){
        motorPosition = 500;
      }else if(initPosStr == "o"){
        motorPosition = 159000;
      }else{
        motorPosition = initPosStr.toInt(); 
      }

      // 입력 성공 시 위치 설정
      if (motorPosition != 0 || initPosStr == "0") {
        Serial.print("초기 위치 설정: ");
        Serial.println(motorPosition);
        return;
      } else {
        Serial.println("유효하지 않은 입력입니다. 정수를 입력하세요.");
        Serial.println("현재 위치를 다시 입력하세요 : ");
      }
    }
  }

  // 타임아웃 처리: 입력 없을 시 0으로 초기화
  Serial.println("입력 시간 초과. 기본 위치(0) 사용.");
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
      stepDelayMicros = 500;
    }
    // 우향으로 이동 명령
    else if (command == "d" && !limitRightTriggered) {
      motorRunning = true;
      motorDirection = HIGH;  // 모터 방향 우향
      digitalWrite(STEP_ENA_PIN, LOW);  // 모터 활성화
      Serial.println("패널 여는중");
    }
    // 좌향으로 이동 명령
    else if (command == "a" && !limitLeftTriggered) {
      motorRunning = true;
      motorDirection = LOW;  // 모터 방향 좌향
      digitalWrite(STEP_ENA_PIN, LOW);  // 모터 활성화
      Serial.println("패널 닫는중");
    }
    // 현재 위치 출력 명령
    else if (command == "p") {
      Serial.print("현재 위치: ");
      Serial.println(motorPosition);
    }
  }

  // 모터가 동작 중일 때
  if (motorRunning) {
    digitalWrite(STEP_DIR_PIN, motorDirection);  // 모터 방향 설정
    digitalWrite(STEP_STP_PIN, HIGH);  // 스텝 신호 HIGH
    delayMicroseconds(stepDelayMicros);  // 스텝 간 딜레이
    digitalWrite(STEP_STP_PIN, LOW);  // 스텝 신호 LOW

    // 모터의 위치 업데이트
    if (motorDirection == HIGH) {
      motorPosition++;
    } else if (motorDirection == LOW) {
      motorPosition--;
    }

    // 모터가 특정 범위에서 자동으로 멈추도록 설정
    if ((motorPosition <= 500 && motorDirection == LOW) || 
        (motorPosition >= 159000 && motorDirection == HIGH)) {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH);
      Serial.println("패널 이동 정지");
    }
  }

  // 리미트 스위치 상태 확인 및 리셋
  if (limitRightTriggered) {
    limitRightTriggered = false;  // 우측 리미트 스위치가 트리거된 후 리셋
  }
  if (limitLeftTriggered) {
    limitLeftTriggered = false;  // 좌측 리미트 스위치가 트리거된 후 리셋
  }
}
