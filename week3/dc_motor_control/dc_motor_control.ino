const int enA = 9;      
const int in1 = 8;
const int in2 = 7;
const int potPin = A0;
const int dirButton = 2;
const int startStopButton = 3;

bool motorRunning = true;
bool direction = true;   

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(dirButton, INPUT_PULLUP);
  pinMode(startStopButton, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  int potValue = analogRead(potPin);
  int speedPWM = map(potValue, 0, 1023, 0, 255);
  int speedPercent = map(speedPWM, 0, 255, 0, 100);

  if (digitalRead(dirButton) == LOW) {
    direction = !direction;
    delay(200);
  }

  if (digitalRead(startStopButton) == LOW) {
    motorRunning = !motorRunning;
    delay(200);
  }

  if (motorRunning) {

    analogWrite(enA, speedPWM);

    if (direction) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }
    else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }

  } else {

    analogWrite(enA, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  Serial.print("Direction: ");
  Serial.print(direction ? "Forward" : "Reverse");

  Serial.print(" | Speed: ");
  Serial.print(speedPercent);
  Serial.print("%");

  Serial.print(" | State: ");
  Serial.println(motorRunning ? "Running" : "Stopped");

  delay(500);
}
