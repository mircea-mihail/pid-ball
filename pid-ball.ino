#include <Servo.h>
Servo servo;
const int pot = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pot, INPUT);
  servo.attach(9);
}
const int maxh = 70, minh = 156;
const int reactionTime = 20, conversionVal = 5.64;

void minMax(){
  servo.write(minh);
  delay(500);
  servo.write(maxh);
  delay(500);
}

void potManualControl(){
  servo.write(analogRead(pot)/conversionVal);  
}

void loop() {
  
}
