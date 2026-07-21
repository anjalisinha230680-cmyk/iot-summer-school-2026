# include<DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
const int ldrPin = A0;
const int trigPin = 9;
const int echoPin = 10;
void setup() {
   Serial.begin(9600);
   dht.begin();
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
}
void loop() {
   float temperature = dht.readTemperature();
   float humidity = dht.readHumidity();
   int lightRaw = analogRead(ldrPin);
   int lightPercent = map(lightRaw,0,1023,0,100);
   String lightCondition;
   if ( lightPercent>=50){
    lightCondition = "Bright";
   }else { 
    lightCondition = "Dark";
   }
   digitalWrite(trigPin,LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin,HIGH);
    float distance = (duration * 0.0343)/2;
    if(isnan(temperature)|| isnan(humidity)){
      Serial.println("Failed to read from DHT22!");
    } else{  
      Serial.println("==== SENSOR LOG ====");
      Serial.print("Time");
      Serial.print(millis());
      Serial.println("ms");
      Serial.println("Temperature:");
      Serial.println(temperature);
      Serial.println("C");
      Serial.print("Humidity:");
      Serial.print(humidity);
      Serial.println("%");
      Serial.println("Light:");
      Serial.println(lightPercent);
      Serial.println("%(");
      Serial.println(lightCondition);
      Serial.println(")");
      Serial.print("Distance:");
      Serial.print(distance);
      Serial.println("cm");
      Serial.println("====================");
    }
    delay(5000);
}
