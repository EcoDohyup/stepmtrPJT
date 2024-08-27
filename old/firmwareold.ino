#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// 모터드라이버 - 
#define STEP_DIR_PIN 12
#define STEP_STP_PIN 14
#define STEP_ENA_PIN 13

#define STEP_DELAY_MICROS 1000 // Default delay between steps

// Variables to store motor state
bool motorRunning = false;
int motorDirection = HIGH; // Default to forward direction
int motorSpeed = 0; // Initial speed

// Replace with your network credentials
const char* ssid = "ecopeace";
const char* password = "ecopeace123";

// Create an instance of the server
AsyncWebServer server(80);


void setup() {
  Serial.begin(115200);
  Serial.println("Stepper Motor Control");

  // Set GPIO pins as outputs
  pinMode(STEP_ENA_PIN, OUTPUT);
  pinMode(STEP_DIR_PIN, OUTPUT);
  pinMode(STEP_STP_PIN, OUTPUT);

  // Disable motor initially
  digitalWrite(STEP_ENA_PIN, HIGH);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");


  server.on("/motor/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    motorRunning = false;
    digitalWrite(STEP_ENA_PIN, HIGH); // Disable motor
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Motor stopped");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    request->send(response);
  });

  server.on("/motor/right", HTTP_GET, [](AsyncWebServerRequest *request){
    motorRunning = true;
    motorDirection = HIGH; // Set direction to right
    digitalWrite(STEP_ENA_PIN, LOW); // Enable motor
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Motor moving right");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    request->send(response);
  });

  server.on("/motor/left", HTTP_GET, [](AsyncWebServerRequest *request){
    motorRunning = true;
    motorDirection = LOW; // Set direction to left
    digitalWrite(STEP_ENA_PIN, LOW); // Enable motor
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Motor moving left");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    request->send(response);
  });

  server.on("/motor/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    String speedParam;
    if (request->hasParam("value")) {
      speedParam = request->getParam("value")->value();
      motorSpeed = speedParam.toInt();
      motorSpeed = max(0, motorSpeed); // Ensure speed is not negative
      AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Speed set to " + speedParam);
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      response->addHeader("Access-Control-Allow-Headers", "Content-Type");
      request->send(response);
    } else {
      AsyncWebServerResponse *response = request->beginResponse(400, "text/plain", "Speed parameter missing");
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      response->addHeader("Access-Control-Allow-Headers", "Content-Type");
      request->send(response);
    }
  });

  // Start server
  server.begin();
}

void loop() {
  if (motorRunning) {
    digitalWrite(STEP_DIR_PIN, motorDirection); // Set direction
    digitalWrite(STEP_STP_PIN, HIGH); // Send step signal
    delayMicroseconds(STEP_DELAY_MICROS / (motorSpeed + 1)); // Adjust speed
    digitalWrite(STEP_STP_PIN, LOW);
  }
}
