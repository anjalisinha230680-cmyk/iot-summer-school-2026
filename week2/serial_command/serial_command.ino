const int ledPin = 13;
int blinkCounter = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Serial Command Interface Ready");
}

void loop() {

  if (Serial.available()) {

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "LED_ON") {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    }

    else if (cmd == "LED_OFF") {
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }

    else if (cmd.startsWith("BLINK_")) {

      int times = cmd.substring(6).toInt();

      if (times >= 1 && times <= 9) {

        for (int i = 0; i < times; i++) {
          digitalWrite(ledPin, HIGH);
          delay(300);
          digitalWrite(ledPin, LOW);
          delay(300);
        }

        blinkCounter += times;
      }

      else {
        Serial.println("ERROR: Unknown command");
      }
    }

    else if (cmd == "STATUS") {
      Serial.print("LED State: ");
      Serial.println(digitalRead(ledPin));

      Serial.print("Blink Counter: ");
      Serial.println(blinkCounter);
    }

    else if (cmd == "RESET") {
      blinkCounter = 0;
      Serial.println("Blink counter reset");
    }

    else {
      Serial.println("ERROR: Unknown command");
    }
  }
}
