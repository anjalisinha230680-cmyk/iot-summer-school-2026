#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Pin Definitions
#define RELAY1 26
#define RELAY2 27
#define BUZZER 25
#define BUTTON1 0
#define BUTTON2 35
#define LED_BT 2

bool light1 = false;
bool light2 = false;

unsigned long lastCommandTime = 0;
const unsigned long timeout = 1800000;   //30 minutes

void beep()
{
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
}

void updateRelay()
{
  digitalWrite(RELAY1, light1 ? LOW : HIGH);   //Active LOW relay
  digitalWrite(RELAY2, light2 ? LOW : HIGH);
}

void sendStatus()
{
  SerialBT.print("L1:");
  SerialBT.print(light1 ? "ON" : "OFF");
  SerialBT.print("  L2:");
  SerialBT.println(light2 ? "ON" : "OFF");
}

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_BT, OUTPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  SerialBT.begin("IIT_IoT_HomeCtrl");

  lastCommandTime = millis();
}

void loop()
{
  // Bluetooth connection indicator
  digitalWrite(LED_BT, SerialBT.hasClient());

  // Bluetooth commands
  if (SerialBT.available())
  {
    char cmd = SerialBT.read();

    switch(cmd)
    {
      case '1':
        light1 = true;
        break;

      case '2':
        light1 = false;
        break;

      case '3':
        light2 = true;
        break;

      case '4':
        light2 = false;
        break;

      case '5':
        light1 = true;
        light2 = true;
        break;

      case '6':
        light1 = false;
        light2 = false;
        break;

      case '?':
        sendStatus();
        break;
    }

    updateRelay();
    beep();
    sendStatus();

    lastCommandTime = millis();
  }
  // Button 1
static bool lastButton1 = LOW;
bool currentButton1 = digitalRead(BUTTON1);

if (currentButton1 == HIGH && lastButton1 == LOW)
{
    light1 = !light1;
    updateRelay();
    beep();
    sendStatus();
    delay(200);
}
lastButton1 = currentButton1;

// Button 2
static bool lastButton2 = LOW;
bool currentButton2 = digitalRead(BUTTON2);

if (currentButton2 == HIGH && lastButton2 == LOW)
{
    light2 = !light2;
    updateRelay();
    beep();
    sendStatus();
    delay(200);
}
lastButton2 = currentButton2;

  
  // Auto OFF after 30 minutes
  if (millis() - lastCommandTime > timeout)
  {
    light1 = false;
    light2 = false;
    updateRelay();
  }
}