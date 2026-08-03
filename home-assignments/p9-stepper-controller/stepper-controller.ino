#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define STEP_PIN 8
#define DIR_PIN 9

#define POT_PIN A0

#define CW_BUTTON   2
#define CCW_BUTTON  3
#define HOME_BUTTON 4

#define LED_PIN 13

const int STEPS_PER_REV = 200;

long currentSteps = 0;
long targetSteps = 0;

float currentAngle = 0;
float targetAngle = 0;

String direction = "STOP";

void stepMotor(int steps, bool dir)
{
  digitalWrite(DIR_PIN, dir);

  for(int i = 0; i < abs(steps); i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(800);

    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(800);
  }
}


void updateDisplay()
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.println("Stepper Positioner");

  display.print("Current:");
  display.print(currentAngle,1);
  display.println((char)247);

  display.print("Target :");
  display.print(targetAngle,1);
  display.println((char)247);

  display.print("Dir    :");
  display.println(direction);

  display.print("Steps  :");
  display.println(currentSteps);

  display.display();
}


void setup()
{
  Serial.begin(9600);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(CW_BUTTON, INPUT_PULLUP);
  pinMode(CCW_BUTTON, INPUT_PULLUP);
  pinMode(HOME_BUTTON, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
  {
    Serial.println("OLED Failed");
    while(1);
  }

  display.clearDisplay();
  display.display();

  Serial.println("Stepper Positioner Ready");
}
