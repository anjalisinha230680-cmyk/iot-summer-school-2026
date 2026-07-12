/*
Author: Anjali Sinha
Date: 12-07-2026
Description: LED blink with serial counter and potentiometer-controlled speed.
*/
int count = 0;
int potPin = A0;
void setup(){
 pinMode(13,OUTPUT);
 Serial.begin(9600);
}
void loop(){
  int delayTime = analogRead(potPin);
  digitalWrite(13,HIGH);
  delay(delayTime);
  digitalWrite(13,LOW);
  delay(delayTime);

 count++;
 Serial.print("Blink count:");
 Serial.println(count);
}
