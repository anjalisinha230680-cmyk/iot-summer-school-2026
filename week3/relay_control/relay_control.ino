#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int relayPin = 8;      
const int buttonPin = 7;     

bool relayState = false;
bool manualOverride = false;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
  dht.begin();
}

void loop() {

  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Sensor Error");
    delay(1000);
    return;
  }


  if (digitalRead(buttonPin) == LOW) {
    manualOverride = !manualOverride;
    relayState = !relayState;

    digitalWrite(relayPin, relayState);

    Serial.print("Manual Override | Temp: ");
    Serial.print(temp);
    Serial.print(" C | Relay: ");
    Serial.println(relayState ? "ON" : "OFF");

    delay(300);
  }

  if (!manualOverride) {

    if (temp > 32 && !relayState) {
      relayState = true;
      digitalWrite(relayPin, HIGH);

      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.println(" C | Relay ON");
    }

    else if (temp < 28 && relayState) {
      relayState = false;
      digitalWrite(relayPin, LOW);

      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.println(" C | Relay OFF");
    }
  }

  delay(1000);
}
