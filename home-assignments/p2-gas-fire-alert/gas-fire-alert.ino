const int gasPin = A0;
const int flameButton = 7;

const int buzzer = 9;
const int greenLED = 10;
const int yellowLED = 11;
const int redLED = 12;

unsigned long previousMillis = 0;
unsigned long serialMillis = 0;

bool buzzerState = false;

void setup() {

  
  pinMode(flameButton, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  Serial.begin(9600);

  
  digitalWrite(greenLED, HIGH);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  noTone(buzzer);

  Serial.println("Gas & Fire Safety Alert System");
  Serial.println("--------------------------------");
}

void loop() {

  

  int gasValue = analogRead(gasPin);

  int gasPercent = map(gasValue, 0, 1023, 0, 100);


  
  bool flameDetected = (digitalRead(flameButton) == LOW);


  unsigned long currentMillis = millis();


  

  if (gasPercent > 60 && flameDetected) {

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

  
    if (currentMillis - previousMillis >= 200) {

      previousMillis = currentMillis;

      buzzerState = !buzzerState;

      if (buzzerState) {
        tone(buzzer, 1500);
      }
      else {
        noTone(buzzer);
      }
    }

    printStatus(gasPercent, "DETECTED", "GAS + FIRE");
  }


  

  else if (flameDetected) {

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

  
    if (currentMillis - previousMillis >= 500) {

      previousMillis = currentMillis;

      buzzerState = !buzzerState;

      if (buzzerState) {
        tone(buzzer, 1000);
      }
      else {
        noTone(buzzer);
      }
    }

    printStatus(gasPercent, "DETECTED", "FIRE");
  }


  

  else if (gasPercent > 60) {

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

  
    tone(buzzer, 1500);

    printStatus(gasPercent, "NONE", "GAS DANGER");
  }


 

  else if (gasPercent >= 30) {

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);

    
    if (currentMillis - previousMillis >= 500) {

      previousMillis = currentMillis;

      buzzerState = !buzzerState;

      if (buzzerState) {
        tone(buzzer, 800);
      }
      else {
        noTone(buzzer);
      }
    }

    printStatus(gasPercent, "NONE", "WARNING");
  }


  
  else {

    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);

    noTone(buzzer);

    buzzerState = false;

    printStatus(gasPercent, "NONE", "SAFE");
  }
}



void printStatus(int gasPercent,
                 const char* flameStatus,
                 const char* status) {

  unsigned long currentMillis = millis();


  if (currentMillis - serialMillis >= 1000) {

    serialMillis = currentMillis;

    Serial.print("GAS: ");
    Serial.print(gasPercent);

    Serial.print("% | FLAME: ");
    Serial.print(flameStatus);

    Serial.print(" | STATUS: ");
    Serial.println(status);
  }
}
