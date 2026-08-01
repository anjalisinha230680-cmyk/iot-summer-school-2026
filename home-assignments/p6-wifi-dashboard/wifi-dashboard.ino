#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <DHTesp.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define DHT_PIN 4
#define LDR_PIN 34
#define I2C_SDA 21
#define I2C_SCL 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


DHTesp dht;
Adafruit_BMP280 bmp; 
AsyncWebServer server(80);


float tempC = 0.0;
float humidity = 0.0;
float pressure = 0.0;
float altitude = 0.0;
int lightPercent = 0;


unsigned long lastSensorRead = 0;
const long sensorInterval = 5000; // 5 seconds

unsigned long lastWifiCheck = 0;
const long wifiCheckInterval = 30000; // 30 seconds


void readSensors();
void handleWifiReconnect();
String buildHtmlPage();
String buildJson();

void setup() {
  Serial.begin(115200);

  
  Wire.begin(I2C_SDA, I2C_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 OLED allocation failed"));
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  
  dht.setup(DHT_PIN, DHTesp::DHT11);

  if (!bmp.begin(0x76) && !bmp.begin(0x77)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
  }
  
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wi-Fi Connected!");
  display.println();
  display.print("SSID: ");
  display.println(ssid);
  display.println();
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", buildHtmlPage());
  });


  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", buildJson());
  });

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();


  if (currentMillis - lastSensorRead >= sensorInterval) {
    lastSensorRead = currentMillis;
    readSensors();
  }

  
  if (currentMillis - lastWifiCheck >= wifiCheckInterval) {
    lastWifiCheck = currentMillis;
    handleWifiReconnect();
  }
}

void readSensors() {
  tempC = dht.getTemperature();
  humidity = dht.getHumidity();

  if (isnan(tempC) || isnan(humidity)) {
    // Fallback to BMP280 temperature if DHT11 fails
    tempC = bmp.readTemperature();
  }

  pressure = bmp.readPressure() / 100.0F; 
  altitude = bmp.readAltitude(1013.25);   

  int ldrRaw = analogRead(LDR_PIN);
  lightPercent = map(ldrRaw, 0, 4095, 0, 100); 
}

// Check Wi-Fi status and attempt reconnect if dropped
void handleWifiReconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.disconnect();
    WiFi.reconnect();
  }
}

// Generate JSON response for /data endpoint
String buildJson() {
  StaticJsonDocument<200> doc;
  doc["temp"] = isnan(tempC) ? 0.0 : tempC;
  doc["humidity"] = isnan(humidity) ? 0.0 : humidity;
  doc["pressure"] = pressure;
  doc["altitude"] = altitude;
  doc["light"] = lightPercent;

  String jsonString;
  serializeJson(doc, jsonString);
  return jsonString;
}


String buildHtmlPage() {
  String bgColor = (tempC > 25.0) ? "#ffe6d5" : "#e6f2ff"; 
  String accentColor = (tempC > 25.0) ? "#d9534f" : "#0275d8";

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<meta http-equiv=\"refresh\" content=\"10\">"; // Auto refresh every 10 seconds
  html += "<title>ESP32 Weather Dashboard</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: " + bgColor + "; text-align: center; margin:0; padding:20px; transition: 0.5s; }";
  html += "h1 { color: " + accentColor + "; }";
  html += ".grid { display: flex; flex-wrap: wrap; justify-content: center; gap: 15px; margin-top: 20px; }";
  html += ".card { background: white; padding: 20px; border-radius: 12px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); width: 160px; }";
  html += ".card h3 { margin: 0 0 10px 0; color: #555; font-size: 16px; }";
  html += ".card p { margin: 0; font-size: 24px; font-weight: bold; color: #333; }";
  html += "</style></head><body>";
  
  html += "<h1>🌤️ Local Weather Station</h1>";
  html += "<p>Live ESP32 Dashboard</p>";
  
  html += "<div class=\"grid\">";
  
  html += "<div class=\"card\"><h3>Temperature</h3><p>" + String(tempC, 1) + " °C</p></div>";
  html += "<div class=\"card\"><h3>Humidity</h3><p>" + String(humidity, 1) + " %</p></div>";
  html += "<div class=\"card\"><h3>Pressure</h3><p>" + String(pressure, 1) + " hPa</p></div>";
  html += "<div class=\"card\"><h3>Altitude</h3><p>" + String(altitude, 1) + " m</p></div>";
  html += "<div class=\"card\"><h3>Light Level</h3><p>" + String(lightPercent) + " %</p></div>";
  
  html += "</div>";
  html += "</body></html>";

  return html;
}
