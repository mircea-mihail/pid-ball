#include <SR04.h>
#include <Servo.h>

const int pot = 0;
const int ultrasonicEcho = 11, ultrasonicTrig = 12;

Servo servo;
SR04 sensor = SR04(ultrasonicEcho, ultrasonicTrig);

const int maxh = 66, minh = 153, midh = 127;
const int reactionTime = 20, conversionVal = 5.64;
const int minl = 0, maxl = 46;
const int avgRate = 100; 
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

double midl(){
  return (maxl + minl)/2;
}

double integratorVal; //integrator value
double y; //raw output
double r = midl(); //refference
double e; //the error (r - y)

double avgV(){
  double sum = 0;
  for(int i = 0; i < avgRate; i++){
    sum += v[i];
  }
  return sum/avgRate;
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

double feedBack(double e){
  if(e > acceptedError){
    //left side
    return - (midh-minh) * errorFunction(e);

  }
  else if(e < -acceptedError){
    //right side
    return - (midh-maxh) * errorFunction(e);
  }
  else{
    return 0;
  }
}

double integrator(double e){
  if(e > acceptedError){
    //left side
    return - (midh-maxh) * errorFunction(e);
  }
  else if(e < -acceptedError){
    //right side
    return - (midh-minh) * errorFunction(e);
  }
  else{
    return 0;
  }
}

const int derivatorFactor = 0.125;
int derivator(){
  return v[avgRate] - v[0];
}

void checkServo(){
  for(int i = minh; i > maxh; i--){
    servo.write(i);
    Serial.println(sensor.Distance());
    delay(100);
  }

  for(int i = maxh; i < minh; i++){
    servo.write(i);
    Serial.println(sensor.Distance());
    delay(100);
  }
}

void loop() {
  //though averageing the y might actually make my 
  //feedback an integrator as averaging it makes it take into account previous information.
  //as such i might end up not using a proportional part as it would be too twitchy?
  //not sure, will see after adding a derivator
  
  //the problem is that the sensor goes crazy when the motor does stuff, so i figured i would give it a delay, to allow the 
  //servo to do its thing and then update the position and it seemed to work. The problem is the derivator is really not helping out.
  
  //the oscilation of the "ball" seems to amplify instead of fizzle out so i'll see what i can do 
  delay(10);

  integratorVal = avgV();
  updateV();
  y = sensor.Distance();

  //0.6 feedback 0.4 integrator 0.125 derivator and add everything up to midh

  //not sure if i made it better or worse

  servo.write(midh + feedBack(r - y) + 0.125 *  derivator());

  //we'll see tomorrow how this goes
  // servo.write(127);
  // potManualControl();
  // Serial.println(analogRead(pot)/conversionVal);
}
