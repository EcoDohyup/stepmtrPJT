#include <CytronMotorDriver.h> 

#define MOTOR_PWM_PIN 14  // PWM pin for controlling motor speed
#define MOTOR_DIR_PIN 27  // Motor direction pin
#define LIMIT_RIGHT 4 // 우극한 리미터 핀
#define LIMIT_LEFT 2 // 좌극한 리미터 핀

CytronMD motor(PWM_DIR, MOTOR_PWM_PIN, MOTOR_DIR_PIN);

// Variables to store motor state
bool motorRunning = false; // 모터 운행상태 표시 변수
int motorDirection = HIGH; // 기본 방향 우향으로 설정
int motorSpeed = 0;

void setup() {
  Serial.begin(115200);  // Initialize serial communication

  // GPIO 초기화
  pinMode(LIMIT_RIGHT, INPUT);
  pinMode(LIMIT_LEFT, INPUT);

  digitalWrite(STEP_ENA_PIN, HIGH);
  digitalWrite(STEP_DIR_PIN, motorDirection); // 기본 우향 세팅
  motor.setSpeed(0);

  
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
      motorSpeed = 0;
      motor.setSpeed(motorSpeed);
      Serial.println("모터 정지");
    } 
    else if (command == "d" && limit_r == HIGH) { // 우향
      motorRunning = true;
      motorSpeed = 15;
      motor.setSpeed(motorSpeed);
      Serial.println("패널 개방중");
    } 
    else if (command == "a" && limit_l == HIGH) { // 좌향
      motorRunning = true;
      motorSpeed = 15;
      motor.setSpeed(motorSpeed);
      Serial.println("패널 닫는중");
    }
  }

  if (motorRunning) {
    
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
