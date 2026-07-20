#include <Keypad.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {A4, A5, 6, 7};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int greenLED = 8;
const int redLED = 9;
const int buzzer = 10;

String correctPIN = "1234";
String enteredPIN = "";

int wrongAttempts = 0;
bool locked = false;
unsigned long lockTime = 0;

void setup() {
  lcd.begin(16, 2);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.print("ENTER PIN:");
}

void loop() {

  if (locked) {
    if (millis() - lockTime >= 10000) {
      locked = false;
      wrongAttempts = 0;
      lcd.clear();
      lcd.print("ENTER PIN:");
    }
    return;
  }

  char key = keypad.getKey();

  if (key) {

    if (key >= '0' && key <= '9') {
      enteredPIN += key;
      lcd.setCursor(0,1);
      lcd.print(enteredPIN);
    }

    if (enteredPIN.length() == 4) {

      lcd.clear();

      if (enteredPIN == correctPIN) {

        lcd.print("ACCESS");
        lcd.setCursor(0,1);
        lcd.print("GRANTED");

        digitalWrite(greenLED, HIGH);
        delay(2000);
        digitalWrite(greenLED, LOW);

        wrongAttempts = 0;
      }

      else {

        lcd.print("ACCESS");
        lcd.setCursor(0,1);
        lcd.print("DENIED");

        digitalWrite(redLED, HIGH);
        tone(buzzer,1000,1000);

        delay(2000);

        digitalWrite(redLED, LOW);

        wrongAttempts++;

        if (wrongAttempts >= 3) {
          lcd.clear();
          lcd.print("LOCKED");
          locked = true;
          lockTime = millis();
        }
      }

      enteredPIN = "";

      if (!locked) {
        lcd.clear();
        lcd.print("ENTER PIN:");
      }
    }
  }
}
