#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#define DHT_PIN       4
#define RED_LED_PIN   25
#define GREEN_LED_PIN 26
#define BUZZER_PIN    27


#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3D 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dht;


unsigned long lastSensorReadTime = 0;
unsigned long lastCsvLogTime = 0;


float temperature = 0.0;
float humidity = 0.0;
String statusLabel = "INIT";

void setup() {
  Serial.begin(115200);

  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  
  dht.setup(DHT_PIN, DHTesp::DHT11);


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed. Check I2C wiring."));
    for (;;); 
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Initializing...");
  display.display();
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  
  if (currentMillis - lastSensorReadTime >= 2000) {
    lastSensorReadTime = currentMillis;

    TempAndHumidity data = dht.getTempAndHumidity();
    
    
    if (dht.getStatus() == DHTesp::ERROR_NONE) {
      temperature = data.temperature;
      humidity = data.humidity;
      
      
      statusLabel = getComfortIndex(temperature, humidity);

      
      bool isAlert = (temperature > 38.0 || humidity > 80.0);
      if (isAlert) {
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
        
        
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000); 
        digitalWrite(BUZZER_PIN, LOW);
      } else {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, LOW);
      }

  
      updateDisplay(temperature, humidity, statusLabel);
    }
  }

  
  if (currentMillis - lastCsvLogTime >= 5000) {
    lastCsvLogTime = currentMillis;
    
    
    Serial.print(currentMillis);
    Serial.print(",");
    Serial.print(temperature, 1);
    Serial.print(",");
    Serial.print(humidity, 1);
    Serial.print(",");
    Serial.println(statusLabel);
  }
}


String getComfortIndex(float temp, float hum) {
  if (temp > 38.0 || hum > 80.0) {
    return "DANGER";
  } else if (temp > 30.0 || hum > 70.0) {
    return "HOT";
  } else if (temp >= 20.0 && temp <= 30.0 && hum >= 30.0 && hum <= 70.0) {
    return "COMFORT";
  } else {
    return "COOL";
  }
}

void updateDisplay(float temp, float hum, String status) {
  display.clearDisplay();

  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");


  display.setCursor(0, 16);
  display.print("Hum:  ");
  display.print(hum, 1);
  display.println(" %");


  display.setCursor(0, 36);
  display.print("Status: ");
  display.setTextSize(2);
  display.setCursor(0, 48);
  display.println(status);

  display.display();
}