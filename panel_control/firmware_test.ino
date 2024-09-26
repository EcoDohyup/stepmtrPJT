#include <CytronMotorDriver.h> 

#define MOTOR_PWM_PIN 14  // PWM pin for controlling motor speed
#define MOTOR_DIR_PIN 27  // Motor direction pin
#define LIMIT_RIGHT 4 // 우극한 리미터 핀
#define LIMIT_LEFT 2 // 좌극한 리미터 핀

// Create motor object from the Cytron library
CytronMD motor(PWM_DIR, MOTOR_PWM_PIN, MOTOR_DIR_PIN);

// Variables to store motor state
bool motorRunning = false; // 모터 운행상태 표시 변수
int motorSpeed = 0; // Motor speed

void setup() {
  Serial.begin(115200);  // Initialize serial communication

  // GPIO initialization
  pinMode(LIMIT_RIGHT, INPUT);
  pinMode(LIMIT_LEFT, INPUT);

  // Start with the motor stopped
  motor.setSpeed(0);  // Set motor speed to 0 initially
  Serial.println("Setup finished");
}

void loop() {
  // Read limit switch states
  int limit_r = digitalRead(LIMIT_RIGHT);
  int limit_l = digitalRead(LIMIT_LEFT);

  // Check for serial input (commands)
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    // Stop the motor
    if (command == "s") {
      motorRunning = false;
      motor.setSpeed(0); // Stop the motor
      Serial.println("모터 정지");
    } 
    // Move motor to the right (positive speed)
    else if (command == "d" && limit_r == HIGH) { // If right limit is not reached
      motorRunning = true;
      motorSpeed = 150;  // Set motor speed to a positive value (adjust as needed)
      motor.setSpeed(motorSpeed);  // Move to the right
      Serial.println("패널 개방중");
    } 
    // Move motor to the left (negative speed)
    else if (command == "a" && limit_l == HIGH) { // If left limit is not reached
      motorRunning = true;
      motorSpeed = -150;  // Set motor speed to a negative value (adjust as needed)
      motor.setSpeed(motorSpeed);  // Move to the left
      Serial.println("패널 닫는중");
    }
  }

  // If the motor is running, check for limit switches
  if (motorRunning) {
    // Stop the motor if the left limit switch is triggered
    if (limit_l == LOW && motorSpeed < 0) { // Moving left and limit switch is hit
      motorRunning = false;
      motor.setSpeed(0); // Stop the motor
      Serial.println("패널 닫기 완료");
    } 
    // Stop the motor if the right limit switch is triggered
    else if (limit_r == LOW && motorSpeed > 0) { // Moving right and limit switch is hit
      motorRunning = false;
      motor.setSpeed(0); // Stop the motor
      Serial.println("패널 개방 완료");
    }
  }
}
