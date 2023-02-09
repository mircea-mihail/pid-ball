#include <SR04.h>
#include <Servo.h>

const int pot = 0;
const int ultrasonicEcho = 11, ultrasonicTrig = 12;

Servo servo;
SR04 sensor = SR04(ultrasonicEcho, ultrasonicTrig);

const int maxh = 66, minh = 153, midh = 127;
const int reactionTime = 20, conversionVal = 5.64;
const int minl = 0, maxl = 46;
const int avgRate = 10; 
int v[avgRate];

void getInitialV(){
  for(int i = 0; i < avgRate; i++){
    long int distance = sensor.Distance();
    if(distance > 50)
      v[i] = 50;
    else 
      v[i] = distance; 
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(pot, INPUT);
  servo.attach(9);
  servo.write(midh);
  getInitialV();
}

// minimum length = 3, maximum length = 46; (distances from the sensor)

void minMax(){
  servo.write(minh);
  delay(500);
  servo.write(maxh);
  delay(500);
}

void potManualControl(){
  servo.write(analogRead(pot)/conversionVal);
}

double y; //raw output
double r; //refference
double e; //the error (r - y)

double avgV(){
  double sum = 0;
  for(int i = 0; i < avgRate; i++){
    sum += v[i];
  }
  return sum/avgRate;
}

double midl(){
  return (maxl + minl)/2;
}

double errorFunction(double e){
  e = abs(e);
  if(e > 20){
    return 1;
  }
  else return (e / 20.0);
}

int vcounter = 0;

void updateV(){
  if(vcounter == avgRate){
    vcounter = 0;
  }
  long int distance = sensor.Distance();
  if(distance > 50)
    v[vcounter] = 50;
  else 
    v[vcounter] = distance; 
  vcounter ++;
}

double servoSafety(double val){
  if(val < minh){
    return minh;
  }
  
  if(val > maxh){
    return maxh;
  }

  return val;
}

const int acceptedError = 2;

double feedBack(){
  if(e > acceptedError){
    //left side
    return midh - (midh-maxh) * errorFunction(e);
  }
  else if(e < -acceptedError){
    //right side
    return midh - (midh-minh) * errorFunction(e);
  }
  else{
    return midh;
  }
}

const int derivatorFactor = 0.125;
double derivator(){
  return e * 0.2;
}

void loop() {

  updateV();
  //though averageing the y might actually make my 
  //feedback an integrator as averaging it makes it take into account previous information.
  //as such i might end up not using a proportional part as it would be too twitchy?
  //not sure, will see after adding a derivator
  y = avgV();
  r = midl();
  e = r - y;

  servo.write(feedBack() + derivator());
  // servo.write(127);
  // potManualControl();
  // Serial.println(analogRead(pot)/conversionVal);
}
