const int redLED = 13;
const int yellowLED = 12;
const int greenLED = 11;
const int buttonPin = 7;
void setup()
{
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
   Serial.begin(9600);
}
void showRed(unsigned long duration){
  digitalWrite(redLED,HIGH);
  digitalWrite(yellowLED,LOW);
  digitalWrite(greenLED,LOW);
  
  Serial.print("RED ON - Time:");
  Serial.println(millis());
  
  delay(duration);
}
void showYellow(unsigned long duration){
  digitalWrite(redLED,LOW);
  digitalWrite(yellowLED,HIGH);
  digitalWrite(greenLED,LOW);
  
  Serial.print("YELLOW ON - Time:");
  Serial.println(millis());
  
  delay(duration);
}
void showGreen(unsigned long duration){
  digitalWrite(redLED,LOW);
  digitalWrite(yellowLED,LOW);
  digitalWrite(greenLED,HIGH);
  
  Serial.print("GREEN ON - Time:");
  Serial.println(millis());
  
  delay(duration);
}


void loop()
{
  if(digitalRead(buttonPin)==LOW){
    Serial.print("PEDESTRIAN CROSSING - RED ON - Time:");
                 Serial.println(millis());
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);              
  delay(8000);
  return;
}
  showRed(5000);
  showYellow(2000);
  showGreen(4000);
 }
