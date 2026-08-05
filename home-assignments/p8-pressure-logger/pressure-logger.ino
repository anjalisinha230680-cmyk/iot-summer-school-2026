#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP280 bmp; 


#define POT_PIN        34  
#define GREEN_LED_PIN  25  
#define RED_LED_PIN    26  


#define LOG_SIZE 24
float pressureLog[LOG_SIZE];
int logIndex = 0;
bool bufferFull = false;

unsigned long lastSensorRead = 0;
unsigned long lastDisplayPageToggle = 0;

const unsigned long READ_INTERVAL = 30000; 
const unsigned long DISPLAY_TOGGLE_INTERVAL = 5000; 

int currentPage = 1;

enum Trend 
{ TREND_STABLE,
 TREND_RISING, 
 TREND_FALLING };
 Trend currentTrend = TREND_STABLE;

float getSeaLevelPressure(float rawPressurePa, float localAltitudeMeters);
void updateTrend();
void updateLEDs();
void displayPage1(float tempC, float pressurehPa, float seaLevelhPa, float baselineAlt);
void displayPage2(float baselineAlt);
void printSerialTable(float tempC, float pressurehPa, float seaLevelhPa, float baselineAlt);

void setup() {
  Serial.begin(115200);
  
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed. Check wiring!"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println(F("Initializing System..."));
  display.display();

  if (!bmp.begin(0x76)) {
    if (!bmp.begin(0x77)) {
      Serial.println(F("Could not find a valid BMP280 sensor! Check I2C address."));
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println(F("BMP280 Error!"));
      display.display();
      for (;;);
    }
  }

  
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     
                  Adafruit_BMP280::SAMPLING_X2,     
                  Adafruit_BMP280::SAMPLING_X16,    
                  Adafruit_BMP280::FILTER_X16,      
                  Adafruit_BMP280::STANDBY_MS_500);

  for (int i = 0; i < LOG_SIZE; i++) {
    pressureLog[i] = 0.0;
  }

  Serial.println(F("Setup complete.\n"));
}

void loop() {
  unsigned long currentMillis = millis();

  
  if (currentMillis - lastSensorRead >= READ_INTERVAL || lastSensorRead == 0) {
    lastSensorRead = currentMillis;

    int potVal = analogRead(POT_PIN);
    float baselineAltitude = map(potVal, 0, 4095, 0, 2000);

    float rawPressurePa = bmp.readPressure(); 
    float rawTempC = bmp.readTemperature();
    
    pressureLog[logIndex % LOG_SIZE] = rawPressurePa;
    logIndex++;
    if (logIndex >= LOG_SIZE) bufferFull = true;

    updateTrend();
    updateLEDs();

    float seaLevelPressurePa = getSeaLevelPressure(rawPressurePa, baselineAltitude);

    printSerialTable(rawTempC, rawPressurePa / 100.0F, seaLevelPressurePa / 100.0F, baselineAltitude);
  }


  if (currentMillis - lastDisplayPageToggle >= DISPLAY_TOGGLE_INTERVAL) {
    lastDisplayPageToggle = currentMillis;
    currentPage = (currentPage == 1) ? 2 : 1;
  }

  int potVal = analogRead(POT_PIN);
  float currentBaselineAlt = map(potVal, 0, 4095, 0, 2000);
  float rawPressPa = bmp.readPressure();
  float seaLevelPa = getSeaLevelPressure(rawPressPa, currentBaselineAlt);
  float actualAltitude = bmp.readAltitude(seaLevelPa/100.0F);
  if (currentPage == 1) {
    displayPage1(bmp.readTemperature(), rawPressPa / 100.0F, seaLevelPa / 100.0F, actualAltitude);
  } else {
    displayPage2(currentBaselineAlt);
  }
}


float getSeaLevelPressure(float rawPressurePa, float localAltitudeMeters) {
  return rawPressurePa / pow(1.0 - (localAltitudeMeters / 44330.0), 5.255);
}

void updateTrend() {
  if (logIndex < 2) {
    currentTrend = TREND_STABLE;
    return;
  }

  float newest = pressureLog[(logIndex - 1) % LOG_SIZE];
  float oldest;

  if (bufferFull) {
    oldest = pressureLog[logIndex % LOG_SIZE]; // Oldest element in full circular buffer
  } else {
    oldest = pressureLog[0]; // First logged element
  }

  float diff = newest - oldest; // Difference in Pascals

  if (diff > 50.0) {
    currentTrend = TREND_RISING;
  } else if (diff < -50.0) {
    currentTrend = TREND_FALLING;
  } else {
    currentTrend = TREND_STABLE;
  }
}

void updateLEDs() {
  if (currentTrend == TREND_FALLING) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  } else { // RISING or STABLE
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  }
}

void displayPage1(float tempC, float pressurehPa, float seaLevelhPa, float actualAltitude) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("--- SENSOR DATA [1/2] ---"));
  
  display.setCursor(0, 16);
  display.print(F("Temp:     ")); display.print(tempC, 1); display.println(F(" C"));
  
  display.setCursor(0, 28);
  display.print(F("Station P:")); display.print(pressurehPa, 1); display.println(F(" hPa"));
  
  display.setCursor(0, 40);
  display.print(F("Sea-Lvl P:")); display.print(seaLevelhPa, 1); display.println(F(" hPa"));

  display.setCursor(0, 52);
  display.print(F("Altitude: ")); display.print( actualAltitude,1); display.println(F(" m"));
  
  display.display();
}

void displayPage2(float baselineAlt) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("--- FORECAST [2/2] ---"));

  display.setCursor(0, 16);
  display.print(F("Trend: "));
  
  display.setTextSize(2);
  display.setCursor(10, 32);
  
  if (currentTrend == TREND_RISING) {
    display.println(F("[ ^ ] RISING"));
    display.setTextSize(1);
    display.setCursor(0, 52);
    display.println(F("Forecast: Good Weather"));
  } else if (currentTrend == TREND_FALLING) {
    display.println(F("[ v ] FALLING"));
    display.setTextSize(1);
    display.setCursor(0, 52);
    display.println(F("Forecast: Rain Likely"));
  } else {
    display.println(F("[ -> ] STABLE"));
    display.setTextSize(1);
    display.setCursor(0, 52);
    display.println(F("Forecast: No Change"));
  }
  
  display.display();
}

void printSerialTable(float tempC, float pressurehPa, float seaLevelhPa, float baselineAlt) {
  Serial.println(F("==============================================================="));
  Serial.println(F("| Log # | Temp(C) | Press(hPa) | SeaLvl(hPa) | Alt(m) | Trend  |"));
  Serial.println(F("==============================================================="));
  Serial.print(F("| "));
  Serial.print(logIndex); if(logIndex < 10) Serial.print(" ");
  Serial.print(F("    | "));
  Serial.print(tempC, 1); Serial.print(F("    | "));
  Serial.print(pressurehPa, 1); Serial.print(F("     | "));
  Serial.print(seaLevelhPa, 1); Serial.print(F("     | "));
  Serial.print((int)baselineAlt); 
  if(baselineAlt < 1000) Serial.print(" ");
  if(baselineAlt < 100) Serial.print(" ");
  Serial.print(F("  | "));
  
  if (currentTrend == TREND_RISING) Serial.println(F("RISING |"));
  else if (currentTrend == TREND_FALLING) Serial.println(F("FALLING|"));
  else Serial.println(F("STABLE |"));
  
  Serial.println(F("===============================================================\n"));
}
  
