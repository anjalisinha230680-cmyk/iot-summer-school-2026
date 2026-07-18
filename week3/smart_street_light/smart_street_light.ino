const int ldrPin = A0;
const int pirPin = 2;
const int ledPin = 9;

const int threshold = 500;

unsigned long motionTime = 0;
bool motionDetected = false;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int ldrValue = analogRead(ldrPin);

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

 
  if (ldrValue > threshold) {

    analogWrite(ledPin, 0);

    Serial.println("[DAY] EVENT: LED OFF");

    motionDetected = false;
  }


  else {

  
    if (digitalRead(pirPin) == HIGH) {

      analogWrite(ledPin, 255);

      motionTime = millis();

      motionDetected = true;

      Serial.println("[NIGHT] EVENT: Motion detected - LED Full Brightness");
    }

    
    if (motionDetected == true) {

      if (millis() - motionTime >= 30000) {

        analogWrite(ledPin, 51);

        Serial.println("[NIGHT] EVENT: No motion - LED Dimmed");

        motionDetected = false;
      }
    }

    else {

      analogWrite(ledPin, 51);
    }
  }

  delay(500);
}
