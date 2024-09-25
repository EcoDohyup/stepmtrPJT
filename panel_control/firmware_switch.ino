using namespace std;
#define STEP_DIR_PIN 12
#define STEP_STP_PIN 14
#define STEP_ENA_PIN 13
#define LIMIT_RIGHT = 4 // 우극한 위치
#define LIMIT_LEFT = 2 // 좌극한 위치
#define STEP_DELAY_MICROS 500 // 속도 아직 기존속도임

// Variables to store motor state
bool motorRunning = false; // 모터 운행상태 표시 변수
int motorDirection = HIGH; // 기본 방향 우향으로 설정
int motorPosition = 0; // 분주비, 사실상 위치값

unsigned long lastPositionSendTime = 0; // 리미트 센서 쓰므로 안 쓸 확률 큼
const unsigned long positionSendInterval = 500; // Interval in milliseconds

void setup() {
  Serial.begin(115200);  // Initialize serial communication

  // GPIO 초기화
  pinMode(STEP_ENA_PIN, OUTPUT);
  pinMode(STEP_DIR_PIN, OUTPUT);
  pinMode(STEP_STP_PIN, OUTPUT);
  pinMode(LIMIT_RIGHT, INPUT);
  pinMode(LIMIT_LEFT, INPUT);
  
}

void loop() {
  // Read sensor value
  int limit_r = digitalRead(LIMIT_RIGHT);
  int limit_l = digitalRead(LIMIT_LEFT);
  
  // Debugging output: Print the sensor pin state
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);
  
  // Check sensor state and print status
  if (sensorValue == LOW) {
    Serial.println("Object detected!");
  } else {
    Serial.println("No object detected.");
  }
  
  delay(500);  // Wait for 500 milliseconds before the next reading
}
