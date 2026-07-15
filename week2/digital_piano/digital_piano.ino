const int buzzer = 8 ;

const int btn1 = 2;
const int btn2 = 3;
const int btn3 = 4;
const int btn4 = 5;

void setup(){
  pinMode(btn1,INPUT_PULLUP);
  pinMode(btn2,INPUT_PULLUP);
  pinMode(btn3,INPUT_PULLUP);
  pinMode(btn4,INPUT_PULLUP);
}
void loop(){
  if(digitalRead(btn1)==LOW)
    tone(buzzer,262);
   else if(digitalRead(btn2)==LOW)
    tone(buzzer,294);
   else if(digitalRead(btn3)==LOW)
    tone(buzzer,330);
   else if(digitalRead(btn4)==LOW)
    tone(buzzer,394);
     
     else
     noTone(buzzer);
     }
