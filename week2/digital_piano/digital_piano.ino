const int buzzer = 8 ;

const int btn1 = 2;
const int btn2 = 3;
const int btn3 = 4;
const int btn4 = 5;
const int modeBtn = 6;

bool major = true;
bool lastState = HIGH;
int majorNotes[] = { 262,294,330,349};
int minorNotes[] = { 262,294,331,349};

void setup(){
  pinMode(btn1,INPUT_PULLUP);
  pinMode(btn2,INPUT_PULLUP);
  pinMode(btn3,INPUT_PULLUP);
  pinMode(btn4,INPUT_PULLUP);
  pinMode(modeBtn,INPUT_PULLUP);
}
void loop(){
  bool current = digitalRead(modeBtn);
  if(lastState == HIGH && current == LOW){
    major = !major;
    delay(200);
  }
  lastState = current;
  int*notes = major?
  majorNotes : minorNotes;
  int pressed = 0;
  if(digitalRead(btn1)==LOW)
    pressed++;
  if(digitalRead(btn2)==LOW)
    pressed++;
  if(digitalRead(btn3)==LOW)
    pressed++;
  if(digitalRead(btn4)==LOW)
    pressed++;
  if( pressed >= 2)
    tone(buzzer,392);
   else if(digitalRead(btn1)==LOW)
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
