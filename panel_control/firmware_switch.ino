using namespace std;
#define STEP_DIR_PIN 12
#define STEP_STP_PIN 14
#define STEP_ENA_PIN 13
#define LIMIT_RIGHT 4 // 우극한 리미터 핀
#define LIMIT_LEFT 2 // 좌극한 리미터 핀
#define STEP_DELAY_MICROS 500 // 속도 아직 기존속도임

// Variables to store motor state
bool motorRunning = false; // 모터 운행상태 표시 변수
int motorDirection = HIGH; // 기본 방향 우향으로 설정

void setup() {
  Serial.begin(115200);  // Initialize serial communication

  // GPIO 초기화
  pinMode(STEP_ENA_PIN, OUTPUT);
  pinMode(STEP_DIR_PIN, OUTPUT);
  pinMode(STEP_STP_PIN, OUTPUT);
  pinMode(LIMIT_RIGHT, INPUT);
  pinMode(LIMIT_LEFT, INPUT);

  digitalWrite(STEP_ENA_PIN, HIGH);
  digitalWrite(STEP_DIR_PIN, motorDirection); // 기본 우향 세팅

  
}

void loop() {
  // Read sensor value
  int limit_r = digitalRead(LIMIT_RIGHT);
  int limit_l = digitalRead(LIMIT_LEFT);

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "s") {
      motorRunning = false;
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 정지
      Serial.println("모터 정지");
    } 
    else if (command == "d" && limit_r == HIGH) { // 우향
      motorRunning = true;
      motorDirection = HIGH; // 모터 우향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
      Serial.println("패널 개방중");
    } 
    else if (command == "a" && limit_l == HIGH) { // 좌향
      motorRunning = true;
      motorDirection = LOW; // 모터 좌향 세팅
      digitalWrite(STEP_ENA_PIN, LOW); // 모터 활성화
      Serial.println("패널 닫는중");
    }
  }

  if (motorRunning) {
    digitalWrite(STEP_DIR_PIN, motorDirection); // 방향 조절
    digitalWrite(STEP_STP_PIN, HIGH); // 분주비 제어
    delayMicroseconds(STEP_DELAY_MICROS); // 펄스간 시간 조정, 고정됨
    digitalWrite(STEP_STP_PIN, LOW); 
    
    if(limit_l == LOW && motorDirection == LOW){ // 좌한 정지
      motorRunning = false; 
      digitalWrite(STEP_ENA_PIN, HIGH); // 모터 완전 정지
      Serial.println("패널 개방 완료");
    } 
    else if(limit_r == LOW && motorDirection == HIGH){ // 우한 정지
      motorRunning = false; 
      digitalWrite(STEP_ENA_PIN, HIGH);
      Serial.println("패널 닫기 완료");
    } 

  }

}
