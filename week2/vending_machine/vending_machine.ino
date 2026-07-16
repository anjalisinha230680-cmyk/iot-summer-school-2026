const int coinBtn = 2;
const int selectBtn = 3;
const int cancelBtn = 4;

const int ledIdle = 8;
const int ledCoin = 9;
const int ledDispense = 10;

enum State {
  IDLE,
  COIN_INSERTED,
  ITEM_SELECTED,
  DISPENSING
};

State currentState = IDLE;

void setup() {
  pinMode(coinBtn, INPUT_PULLUP);
  pinMode(selectBtn, INPUT_PULLUP);
  pinMode(cancelBtn, INPUT_PULLUP);

  pinMode(ledIdle, OUTPUT);
  pinMode(ledCoin, OUTPUT);
  pinMode(ledDispense, OUTPUT);

  Serial.begin(9600);

  updateLEDs();
  Serial.println("State: IDLE");
}

void loop() {

  if (digitalRead(cancelBtn) == LOW) {
    currentState = IDLE;
    updateLEDs();
    Serial.println("State -> IDLE");
    delay(300);
  }

  switch (currentState) {

    case IDLE:
      if (digitalRead(coinBtn) == LOW) {
        currentState = COIN_INSERTED;
        updateLEDs();
        Serial.println("State -> COIN_INSERTED");
        delay(300);
      }
      break;

    case COIN_INSERTED:
      if (digitalRead(selectBtn) == LOW) {
        currentState = ITEM_SELECTED;
        updateLEDs();
        Serial.println("State -> ITEM_SELECTED");
        delay(300);
      }
      break;

    case ITEM_SELECTED:
      currentState = DISPENSING;
      updateLEDs();
      Serial.println("State -> DISPENSING");

      delay(2000);

      currentState = IDLE;
      updateLEDs();
      Serial.println("State -> IDLE");
      break;

    case DISPENSING:
      break;
  }
}

void updateLEDs() {

  digitalWrite(ledIdle, LOW);
  digitalWrite(ledCoin, LOW);
  digitalWrite(ledDispense, LOW);

  if (currentState == IDLE) {
    digitalWrite(ledIdle, HIGH);
  }
  else if (currentState == COIN_INSERTED) {
    digitalWrite(ledCoin, HIGH);
  }
  else {
    digitalWrite(ledDispense, HIGH);
  }
}
