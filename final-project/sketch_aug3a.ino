#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <AccelStepper.h>

// ==========================================
// PIN DEFINITIONS
// ==========================================
#define DHTPIN         4
#define FLAME_PIN      5
#define RGB_BLUE_PIN   12
#define LDR_PIN        13
#define RGB_GREEN_PIN  14
#define TOUCH_PIN      15
#define STEPPER_IN1    16
#define STEPPER_IN2    17
#define SERVO_PIN      18
#define REED_PIN       19
#define OLED_SDA       21
#define OLED_SCL       22
#define BUTTON_PIN     23
#define BUZZER_PIN     25
#define RGB_RED_PIN    26
#define RELAY_PIN      27
#define SHOCK_PIN      32
#define SOUND_PIN      33
#define IR_PIN         34

// ==========================================
// CONFIGURATION & CONSTANTS
// ==========================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DHTTYPE DHT11

// Stepper Motor Pins
#define STEPPER_IN3 2
#define STEPPER_IN4 0

// Object Instantiation
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);
Servo doorServo;

// Stepper Motor Object
AccelStepper gate(
    AccelStepper::FULL4WIRE,
    STEPPER_IN1,
    STEPPER_IN3,
    STEPPER_IN2,
    STEPPER_IN4
);

// ==========================================
// GLOBAL STATE VARIABLES
// ==========================================
bool isArmed = true;             // Security armed state
bool alarmActive = false;         // Emergency trigger status
String alertCause = "";           // Cause of emergency

// OLED Page Navigation
enum DisplayPage { PAGE_HOME, PAGE_SECURITY, PAGE_EMERGENCY };
DisplayPage currentPage = PAGE_HOME;
unsigned long lastPageSwitch = 0;

// Non-blocking Timers
unsigned long lastDHTRead = 0;
unsigned long doorOpenTime = 0;
unsigned long gateOpenTime = 0;

// Sensor States
float temperature = 0.0;
float humidity = 0.0;
bool doorOpen = false;
bool motionDetected = false;
bool fireDetected = false;
bool shockDetected = false;
bool soundDetected = false;

// Debounce for Touch Sensor
bool lastTouchState = LOW;
unsigned long lastTouchTime = 0;

// ==========================================
// HELPER FUNCTIONS
// ==========================================

void setRGB(bool r, bool g, bool b) {
  digitalWrite(RGB_RED_PIN, r ? HIGH : LOW);
  digitalWrite(RGB_GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(RGB_BLUE_PIN, b ? HIGH : LOW);
}

void lockDoor() {
  doorServo.write(0); // Locked position
}

void unlockDoor() {
  doorServo.write(90); // Unlocked position
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);

  // Initialize Sensors & Actuators
  pinMode(FLAME_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(SHOCK_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Servo Setup
  doorServo.attach(SERVO_PIN);
  lockDoor();

  // Stepper Setup
  gate.setMaxSpeed(500.0);
  gate.setAcceleration(200.0);

  // DHT Setup
  dht.begin();

  // OLED Setup
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Allocation Failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.print(F("Initializing..."));
  display.display();
  delay(1000);
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {
  // 1. Read Environmental Data Periodic Refresh (Every 2 seconds)
  if (millis() - lastDHTRead > 2000) {
    lastDHTRead = millis();
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
  }

  // 2. Read All Digital Security Sensors
  fireDetected  = (digitalRead(FLAME_PIN) == HIGH); // Active LOW on most flame sensors
  doorOpen      = (digitalRead(REED_PIN) == HIGH); // Magnet away = OPEN
  motionDetected= (digitalRead(IR_PIN) == LOW);    // Active LOW when object detected
  shockDetected = (digitalRead(SHOCK_PIN) == HIGH);
  soundDetected = (digitalRead(SOUND_PIN) == LOW);
  bool isDark   = (digitalRead(LDR_PIN) == HIGH);   // High = Dark (dependent on LDR module setup)

  // 3. Module 2: Smart Lighting Control
  digitalWrite(RELAY_PIN, ! isArmed ? HIGH : LOW);

  // 4. Module 8: Touch Security Toggle (Arm/Disarm)
  bool touchState = digitalRead(TOUCH_PIN);
  if (touchState == HIGH && lastTouchState == LOW && (millis() - lastTouchTime > 300)) {
  isArmed = !isArmed;
  lastTouchTime = millis();

  if (!isArmed) {
    unlockDoor();
    //gate.moveTo(2048);
    //gateOpenTime = millis();
  } else {
    lockDoor();
  }
}
  lastTouchState = touchState;

  
  // 5. Security Check & Alarm Trigger Logic
alarmActive = false;
alertCause = "";

if (fireDetected) {
  alarmActive = true;
  alertCause = "FIRE ALERT";
}
else if (isArmed) {
  if (doorOpen) {
    alarmActive = true;
    alertCause = "DOOR BREACH";
  }
  else if (motionDetected) {
    alarmActive = true;
    alertCause = "INTRUDER";
  }
  else if (shockDetected) {
    alarmActive = true;
    alertCause = "VIBRATION";
  }
  else if (soundDetected) {
    alarmActive = true;
    alertCause = "LOUD NOISE";
  }
}

  // 6. Actuator Responses
  if (alarmActive) {
    digitalWrite(BUZZER_PIN, HIGH);
    setRGB(true, false, false); // RED
    lockDoor();                 // Lock door instantly on threat
    currentPage = PAGE_EMERGENCY;
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    if (isArmed) {
      setRGB(false, true, false); // GREEN (Armed & Safe)
    } else {
      setRGB(true, true, false);  // YELLOW (Disarmed/Warning)
    }
  }

  // Auto-close Gate Timer (If disarmed and gate opened)
  if (!isArmed && gate.distanceToGo() == 0 && gate.currentPosition() == 2048) {
    if (millis() - gateOpenTime > 5000) { // Keep gate open for 5 sec
      gate.moveTo(0); // Return to closed position
    }
  }

  // Module 3: Auto-lock Door Logic
  if (doorOpen) {
  doorOpenTime = millis();
} else {
  if (millis() - doorOpenTime > 3000 && !alarmActive && isArmed) {
    lockDoor();
  }
}

  // Run Stepper Motor Step Execution
  gate.run();

  // 7. Update Screen Pages (Rotate Home/Security if not Emergency)
  if (!alarmActive) {
    if (millis() - lastPageSwitch > 3000) { // Cycle every 3 seconds
      lastPageSwitch = millis();
      currentPage = (currentPage == PAGE_HOME) ? PAGE_SECURITY : PAGE_HOME;
    }
  }

  updateDisplay();
  Serial.println("\n========== SENSOR STATUS ==========");

Serial.print("Temperature: ");
Serial.print(temperature);
Serial.println(" C");

Serial.print("Humidity: ");
Serial.print(humidity);
Serial.println(" %");

Serial.print("Flame: ");
Serial.println(fireDetected ? "DETECTED" : "SAFE");

Serial.print("LDR: ");
Serial.println(isDark ? "DARK" : "BRIGHT");

Serial.print("Touch: ");
Serial.println(touchState ? "TOUCHED" : "NOT TOUCHED");

Serial.print("Door: ");
Serial.println(doorOpen ? "OPEN" : "CLOSED");

Serial.print("IR Tracking: ");
Serial.println(motionDetected ? "OBJECT DETECTED" : "CLEAR");

Serial.print("Shock: ");
Serial.println(shockDetected ? "DETECTED" : "NO");

Serial.print("Sound: ");
Serial.println(soundDetected ? "LOUD" : "NORMAL");

Serial.print("Security: ");
Serial.println(isArmed ? "ARMED" : "DISARMED");

Serial.print("Relay: ");
Serial.println(digitalRead(RELAY_PIN) ? "ON" : "OFF");

Serial.print("Alarm: ");
Serial.println(alarmActive ? "ACTIVE" : "OFF");


Serial.println("===================================\n");

delay(1000);

}

// ==========================================
// OLED DISPLAY HANDLER
// ==========================================
void updateDisplay() {
  display.clearDisplay();

  if (currentPage == PAGE_EMERGENCY) {
    display.setTextSize(2);
    display.setCursor(10, 0);
    display.print(F("!!ALERT!!"));
    
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print(F("CAUSE: "));
    display.print(alertCause);

    display.setCursor(0, 45);
    display.print(F("BUZZER : ACTIVE"));
  } 
  else if (currentPage == PAGE_HOME) {
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print(F("SMART HOME"));
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    display.setCursor(0, 20);
    display.print(F("Temp     : "));
    display.print(isnan(temperature) ? 0 : temperature, 1);
    display.print(F(" C"));

    display.setCursor(0, 35);
    display.print(F("Humidity : "));
    display.print(isnan(humidity) ? 0 : humidity, 1);
    display.print(F(" %"));

    display.setCursor(0, 50);
    display.print(F("Security : "));
    display.print(isArmed ? F("ARMED") : F("DISARMED"));
  } 
  else if (currentPage == PAGE_SECURITY) {
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print(F("SECURITY STATUS"));
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    display.setCursor(0, 18);
    display.print(F("Door   : ")); display.print(doorOpen ? F("OPEN") : F("Closed"));
    
    display.setCursor(0, 30);
    display.print(F("Motion : ")); display.print(motionDetected ? F("DETECTED") : F("Safe"));

    display.setCursor(0, 42);
    display.print(F("Fire   : ")); display.print(fireDetected ? F("DETECTED") : F("Safe"));

    display.setCursor(0, 54);
    display.print(F("Alarm  : ")); display.print(alarmActive ? F("ON") : F("OFF"));
  }

  display.display();
}