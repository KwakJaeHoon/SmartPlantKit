//2016.06.06 MON
// Sogang Univ. Capstone Design I Class
// 20111587 Kwak Jae Hoon

#include "Timer.h"
Timer t;
#include <OneWire.h>
#include <DallasTemperature.h>
#define RELAY1 13 // Spray Pin number
#define RELAY2 12 // LED bulb Pin number
#define RELAY3 7
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#include <SoftwareSerial.h>
SoftwareSerial mySerial(5,6);
int no_water_flag = 0;
int serial = 0;
int HumidityPin = A0;
int lightPin = A1;
int humAlaram = 0;

int BuzzerPin = 9;
int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};
             // C    D    E    F    G    A    B    C

void repeat()
{
  // get input from digital 8
  sensors.requestTemperatures();

  // store the analog value from humidity sensor
  // get input from analog 0
  int humVal = analogRead(HumidityPin);
  // get input from analog 1
  int lightVal = analogRead(lightPin);
  Serial.print("Humidity: ");
  Serial.print(humVal);
  Serial.print(", ");
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print(", ");
  Serial.print("Light: ");
  Serial.print(lightVal);
  Serial.print("\n");

  if(sensors.getTempCByIndex(0) >= 30){
    digitalWrite(RELAY3, HIGH);
  }
  else{
    digitalWrite(RELAY3, LOW);
  }
  
  if(lightVal >= 1000){
      digitalWrite(RELAY2, LOW);
  }
  else{
    digitalWrite(RELAY2, HIGH);
  }
  if(humVal == 0) // If Humidity of land
  {
    no_water_flag += 1;
    if(no_water_flag > 60){
      tone(BuzzerPin, tones[1]); // Buzzer on
      delay(2000);
      noTone(BuzzerPin);
    }
    else{
      digitalWrite(RELAY1, LOW); // Spray on
      tone(BuzzerPin, tones[0]); // Buzzer on
      delay(2000); // 2 sec
      noTone(BuzzerPin);          // Buzzer off
      digitalWrite(RELAY1, HIGH); // Spray off
    }
  }
  else{
    no_water_flag = 0;
  }
  
  // kudoino to nodemcu
  mySerial.println("&tempvalue="+ String(sensors.getTempCByIndex(0)) + "&serial=" + String(serial++) + "&humvalue=" + String(humVal) + "&lightvalue=" + String(lightVal) + "&no_water_flag="+String(no_water_flag));
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Sensor node V1.0 2016/5/11 ");
  
  mySerial.begin(9600);
  sensors.begin();
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, HIGH);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY2, HIGH);
  pinMode(RELAY3, OUTPUT);
  t.every(1000, repeat);
}
void loop()
{
  t.update();
  while(mySerial.available()) {Serial.print((char)mySerial.read());}
}
