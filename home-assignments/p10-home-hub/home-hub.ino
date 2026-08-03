#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";


const char* MQTT_BROKER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC  = "iitjammu/student/home"; // Replace 'student' with your name

#define DHTPIN        4
#define DHTTYPE       DHT11
#define MQ2_PIN       35
#define LDR_PIN       34
#define PIR_PIN       13
#define RELAY_FAN     26
#define RELAY_LIGHT   27
#define BUZZER_PIN    14
#define LED_RED       25
#define LED_GREEN     2           
#define BTN_FAN       0
#define BTN_LIGHT     32

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64


#define RELAY_ON  LOW
#define RELAY_OFF HIGH


const unsigned long SENSOR_INTERVAL      = 5000;    
const unsigned long MQTT_INTERVAL        = 30000;  
const unsigned long OLED_INTERVAL        = 5000;    
const unsigned long OVERRIDE_DURATION    = 600000;  
const unsigned long LIGHT_OFF_TIMEOUT    = 180000;  

float temperature = 0.0;
float humidity = 0.0;
float gasPercentage = 0.0;
float lightPercentage = 0.0;
bool pirState = false;

bool fanRelayState = false;  
bool lightRelayState = false;
bool gasAlertState = false;


bool fanManualOverride = false;
unsigned long fanOverrideStartTime = 0;

bool lightManualOverride = false;
unsigned long lightOverrideStartTime = 0;


unsigned long lastMotionTime = 0;


unsigned long lastSensorRead = 0;
unsigned long lastMqttPub = 0;
unsigned long lastOledCycle = 0;
uint8_t oledScreen = 0;


DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
AsyncWebServer server(80);


String getUptimeString() {
  unsigned long totalSec = millis() / 1000;
  int days = totalSec / 86400;
  int hours = (totalSec % 86400) / 3600;
  int minutes = (totalSec % 3600) / 60;
  int seconds = totalSec % 60;
  
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%dd %02dh:%02dm:%02ds", days, hours, minutes, seconds);
  return String(buffer);
}

void readSensors() {
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t)) temperature = t;
  if (!isnan(h)) humidity = h;

  
  int rawGas = analogRead(MQ2_PIN);
  gasPercentage = (rawGas / 4095.0) * 100.0;

  int rawLdr = analogRead(LDR_PIN);
  lightPercentage = (1.0 - (rawLdr / 4095.0)) * 100.0;

  
  pirState = digitalRead(PIR_PIN);
  if (pirState) {
    lastMotionTime = millis();
  }
}

void processAutomationRules() {
  unsigned long now = millis();

  
  if (gasPercentage > 60.0) {
    gasAlertState = true;
    
    
    fanRelayState = false;
    lightRelayState = false;
    digitalWrite(RELAY_FAN, RELAY_OFF);
    digitalWrite(RELAY_LIGHT, RELAY_OFF);
    
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    return; // Bypass normal automation rules
  } else {
    gasAlertState = false;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  }

 
  if (fanManualOverride) {
    if (now - fanOverrideStartTime >= OVERRIDE_DURATION) {
      fanManualOverride = false; // Expire override after 10 mins
    }
  } 
  
  if (!fanManualOverride) {
    // Hysteresis logic
    if (!fanRelayState && temperature > 32.0) {
      fanRelayState = true;
    } else if (fanRelayState && temperature < 28.0) {
      fanRelayState = false;
    }
  }

  if (lightManualOverride) {
    if (now - lightOverrideStartTime >= OVERRIDE_DURATION) {
      lightManualOverride = false; 
    }
  }

  if (!lightManualOverride) {
    bool motionRecently = (now - lastMotionTime < LIGHT_OFF_TIMEOUT);
    
    if (lightPercentage < 20.0 && pirState) {
      lightRelayState = true;
    } else if (lightPercentage > 60.0 || !motionRecently) {
      lightRelayState = false;
    }
  }

  digitalWrite(RELAY_FAN, fanRelayState ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY_LIGHT, lightRelayState ? RELAY_ON : RELAY_OFF);
}

void checkButtons() {
  if (digitalRead(BTN_FAN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BTN_FAN) == LOW) {
      fanManualOverride = true;
      fanOverrideStartTime = millis();
      fanRelayState = !fanRelayState; 
      digitalWrite(RELAY_FAN, fanRelayState ? RELAY_ON : RELAY_OFF);
      while(digitalRead(BTN_FAN) == LOW); 
    }
  }

  if (digitalRead(BTN_LIGHT) == LOW) {
    delay(50); 
    if (digitalRead(BTN_LIGHT) == LOW) {
      lightManualOverride = true;
      lightOverrideStartTime = millis();
      lightRelayState = !lightRelayState; 
      digitalWrite(RELAY_LIGHT, lightRelayState ? RELAY_ON : RELAY_OFF);
      while(digitalRead(BTN_LIGHT) == LOW); 
    }
  }
}

void updateOLED() {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  switch (oledScreen) {
    case 0: 
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.println("=== CLIMATE ===");
      oled.setTextSize(2);
      oled.setCursor(0, 18);
      oled.printf("T: %.1f C\n", temperature);
      oled.setCursor(0, 42);
      oled.printf("H: %.1f %%\n", humidity);
      break;

    case 1: 
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.println("=== SENSORS ===");
      oled.setCursor(0, 18);
      oled.printf("Gas Level : %.1f%%\n", gasPercentage);
      oled.setCursor(0, 32);
      oled.printf("Light Lvl : %.1f%%\n", lightPercentage);
      oled.setCursor(0, 46);
      oled.printf("Motion    : %s\n", pirState ? "DETECTED" : "CLEAR");
      break;

    case 2: 
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.println("=== SYSTEM STATE ===");
      oled.setCursor(0, 16);
      oled.printf("Fan  : %s %s\n", fanRelayState ? "ON " : "OFF", fanManualOverride ? "(M)" : "(A)");
      oled.setCursor(0, 28);
      oled.printf("Light: %s %s\n", lightRelayState ? "ON " : "OFF", lightManualOverride ? "(M)" : "(A)");
      oled.setCursor(0, 44);
      oled.printf("Up: %s", getUptimeString().c_str());
      break;
  }
  oled.display();
}

void publishMQTT() {
  if (!mqttClient.connected()) return;

  StaticJsonDocument<256> doc;
  doc["temp"]        = temperature;
  doc["humidity"]    = humidity;
  doc["gas"]         = gasPercentage;
  doc["pir"]         = pirState;
  doc["light"]       = lightPercentage;
  doc["fan"]         = fanRelayState ? "ON" : "OFF";
  doc["light_relay"] = lightRelayState ? "ON" : "OFF";
  doc["alert"]       = gasAlertState ? "GAS ALERT!" : "NORMAL";

  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);
  mqttClient.publish(MQTT_TOPIC, jsonBuffer);
}

void setupWebDashboard() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = R"rawliteral(
    <!DOCTYPE html><html><head><title>IoT Home Hub</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial, sans-serif; background: #121212; color: #fff; text-align: center; margin:0; padding:20px;}
      .card { background: #1e1e1e; padding: 15px; margin: 10px auto; max-width: 400px; border-radius: 8px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }
      .btn { padding: 10px 20px; font-size: 16px; border: none; border-radius: 5px; cursor: pointer; color: white; margin-top: 10px; }
      .btn-on { background-color: #28a745; } .btn-off { background-color: #dc3545; }
      .alert { color: #ff4d4d; font-weight: bold; }
    </style>
    <script>
      setInterval(() => { fetch('/data').then(r => r.json()).then(d => {
        document.getElementById('temp').innerText = d.temp + ' °C';
        document.getElementById('hum').innerText = d.humidity + ' %';
        document.getElementById('gas').innerText = d.gas + ' %';
        document.getElementById('light').innerText = d.light + ' %';
        document.getElementById('pir').innerText = d.pir ? 'Motion Detected' : 'Clear';
        document.getElementById('fan').innerText = d.fan + (d.fanOverride ? ' (Manual)' : ' (Auto)');
        document.getElementById('lightRelay').innerText = d.lightRelay + (d.lightOverride ? ' (Manual)' : ' (Auto)');
        document.getElementById('uptime').innerText = d.uptime;
        document.getElementById('alert').innerText = d.alert;
      }); }, 2000);
    </script></head><body>
      <h2>IoT Hostel Hub Dashboard</h2>
      <div class="card"><h3>Alert Status</h3><p id="alert" class="alert">--</p></div>
      <div class="card">
        <h3>Environment</h3>
        <p>Temp: <span id="temp">--</span> | Humidity: <span id="hum">--</span></p>
        <p>Gas: <span id="gas">--</span> | Light: <span id="light">--</span></p>
        <p>PIR: <span id="pir">--</span></p>
      </div>
      <div class="card">
        <h3>Controls</h3>
        <p>Fan: <span id="fan">--</span></p>
        <a href="/toggle/fan"><button class="btn btn-on">Toggle Fan Override</button></a>
        <p>Light: <span id="lightRelay">--</span></p>
        <a href="/toggle/light"><button class="btn btn-on">Toggle Light Override</button></a>
      </div>
      <div class="card"><p>Uptime: <span id="uptime">--</span></p></div>
    </body></html>)rawliteral";
    request->send(200, "text/html", html);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["temp"]          = temperature;
    doc["humidity"]      = humidity;
    doc["gas"]           = gasPercentage;
    doc["light"]         = lightPercentage;
    doc["pir"]           = pirState;
    doc["fan"]           = fanRelayState ? "ON" : "OFF";
    doc["fanOverride"]   = fanManualOverride;
    doc["lightRelay"]    = lightRelayState ? "ON" : "OFF";
    doc["lightOverride"] = lightManualOverride;
    doc["uptime"]        = getUptimeString();
    doc["alert"]         = gasAlertState ? "DANGER: HIGH GAS LEVEL!" : "SYSTEM NORMAL";

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/toggle/fan", HTTP_GET, [](AsyncWebServerRequest *request){
    fanManualOverride = true;
    fanOverrideStartTime = millis();
    fanRelayState = !fanRelayState;
    digitalWrite(RELAY_FAN, fanRelayState ? RELAY_ON : RELAY_OFF);
    request->redirect("/");
  });

  server.on("/toggle/light", HTTP_GET, [](AsyncWebServerRequest *request){
    lightManualOverride = true;
    lightOverrideStartTime = millis();
    lightRelayState = !lightRelayState;
    digitalWrite(RELAY_LIGHT, lightRelayState ? RELAY_ON : RELAY_OFF);
    request->redirect("/");
  });

  server.begin();
}

void reconnectMQTT() {
  if (!mqttClient.connected()) {
    String clientId = "ESP32Hub-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      publishMQTT();
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  digitalWrite(RELAY_FAN, RELAY_OFF);
  digitalWrite(RELAY_LIGHT, RELAY_OFF);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);

  pinMode(PIR_PIN, INPUT);
  pinMode(BTN_FAN, INPUT_PULLUP);
  pinMode(BTN_LIGHT, INPUT_PULLUP);

  dht.begin();
  Wire.begin(21, 22);
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  setupWebDashboard();
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  checkButtons();

  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorRead >= SENSOR_INTERVAL) {
    readSensors();
    processAutomationRules();
    lastSensorRead = currentMillis;
  }

  if (currentMillis - lastOledCycle >= OLED_INTERVAL) {
    oledScreen = (oledScreen + 1) % 3;
    updateOLED();
    lastOledCycle = currentMillis;
  }

  if (currentMillis - lastMqttPub >= MQTT_INTERVAL) {
    publishMQTT();
    lastMqttPub = currentMillis;
  }
}
