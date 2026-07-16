const int led = 9;
const int button = 2;

int mode = 0;
bool lastButton = HIGH;
void setup(){
  pinMode(led,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Mode 1 : Slow Breathing");
}
void loop(){
  bool currentButton = digitalRead(button);
  
  if(lastButton == HIGH && currentButton == LOW){
    mode++;
    if(mode>2) mode = 0;
    if(mode ==0)
      Serial.println("Mode 1 : Slow Breathing");
   else if(mode ==1)
      Serial.println("Mode 2 : fast pulse");
    else
      Serial.println("Mode 3 : SOS");
     delay(200);
  }
  lastButton = currentButton;
  if(mode == 0)
    slowBreathing();
  else if(mode == 1)
    fastpulse();
    else 
     sosPattern();
}
   void slowBreathing(){
     for( int i = 0;i<255;i++)
     { 
       analogWrite(led,i);
       delay(6);
     }
     for(int i = 225;i>=0;i--)
     {
       analogWrite(led,i);
       delay(6);
     }
   }
   void fastpulse(){
     for( int i = 0;i<255;i++)
     { 
       analogWrite(led,i);
       delay(1);
     }
     for(int i = 225;i>=0;i--)
     {
       analogWrite(led,i);
       delay(1);
     }
   }
    void sosPattern(){
      for(int i =0;i<3;i++){
        digitalWrite(led,HIGH);
         delay(200);
        digitalWrite(led,LOW);
         delay(200);
      }
      for(int i =0;i<3;i++){
        digitalWrite(led,HIGH);
         delay(600);
        digitalWrite(led,LOW);
         delay(200);
      }
      for(int i =0;i<3;i++){
        digitalWrite(led,HIGH);
         delay(200);
        digitalWrite(led,LOW);
         delay(200);
      }
      delay(1000);
    }
    
