const int trigPin = 9;
const int echoPin = 10;
const int greenLED = 6;
const int yellowLED = 7;
const int redLED = 8;
const int buzzer = 5;
unsigned long previousMillis = 0;
bool beepState = false;
void setup(){
pinMode(trigPin,OUTPUT);
pinMode(echoPin,INPUT);
pinMode(greenLED,OUTPUT);
pinMode(yellowLED,OUTPUT);
pinMode(redLED,OUTPUT);
pinMode(buzzer,OUTPUT);
Serial.begin(9600);
}
void loop(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float distance = (duration * 0.034) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  unsigned long currentMillis = millis();

  if (distance > 50) {

    Serial.println("SAFE");

    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    noTone(buzzer);
  }

  else if (distance > 20 && distance <= 50) {

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);

    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      beepState = !beepState;

      if (beepState)
        tone(buzzer, 1000);
      else
        noTone(buzzer);
    }
  }

  else if (distance > 10 && distance <= 20) {

    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

    if (currentMillis - previousMillis >= 200) {
      previousMillis = currentMillis;
      beepState = !beepState;

      if (beepState)
        tone(buzzer, 1000);
      else
        noTone(buzzer);
    }
  }

  else {

    if (currentMillis - previousMillis >= 100) {

      previousMillis = currentMillis;

      beepState = !beepState;

      digitalWrite(greenLED, beepState);
      digitalWrite(yellowLED, beepState);
      digitalWrite(redLED, beepState);

      if (beepState)
        tone(buzzer, 1000);
      else
        noTone(buzzer);
    }
  }
}
