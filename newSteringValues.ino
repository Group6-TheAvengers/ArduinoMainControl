#include <Smartcar.h>
/*
   Full Serial control via bluetooth, the car will only move by rotating it after encoutering an obstacle
*/
Car car;
Odometer encoderLeft, encoderRight;
Gyroscope gyro;
SR04 ultrasonicSensor1;
SR04 ultrasonicSensor2;
int speed, front, back, angle;
char in;
float dis;
float currentSpeed;
// Used for the line following
const int MOTOR_MAX_SPEED=50;
const int MOTOR_BASE_SPEED=35;
float Kp=10,Kd=80;
float currError=0;
float correction=0;
float lastError=0;
float leftin=0,rightin=0,leftout=0,rightout=0;
boolean follow;

void setup() {
  Serial3.begin(9600);
  gyro.attach();
  encoderLeft.attach(2);
  encoderRight.attach(3);
  encoderLeft.begin();
  encoderRight.begin();
  gyro.begin();
  car.begin(encoderLeft, encoderRight, gyro);
  ultrasonicSensor1.attach(9, 10);
  ultrasonicSensor2.attach(6, 5);
  in = 0;
  speed = 0;
  front = 0;
  back = 0;
  angle = 0;
  dis = 0;
  currentSpeed = 0;
  boolean follow = false;
}


void loop() {
  if (Serial3.available()) {
    in = Serial3.read();
    handleInput();
  }
  
  if(follow == true) {
     FollowLine();
  }
  
   currentSpeed = (encoderRight.getSpeed() + encoderLeft.getSpeed())/2;
   
  Serial3.print("s");
  if(currentSpeed < 0.2){
    currentSpeed = 0;
    Serial3.println(currentSpeed);
  }else{
  Serial3.println(currentSpeed);
  }
  dis = encoderRight.getDistance();
  Serial3.print("d");
  Serial3.println(dis / 100);
}

void handleInput() { //handle serial input if there is any
  

  //Commands from android
  switch (in) {
    //Forward speeds
    case 'a':
      speed = 30;
      car.setSpeed(speed);
      break;
    case 'b':
      speed = 60;
      car.setSpeed(speed);
      break;
    case 'c':
      speed = 100;
      car.setSpeed(speed);
      break;
    //Backward speeds
    case 'd':
      speed = -30;
      car.setSpeed(speed);
      break;
    case 'e':
      speed = -60;
      car.setSpeed(speed);
      break;
    case 'f':
      speed = -100;
      car.setSpeed(speed);
      break;
    //Right angles
    case 'g':
      car.setAngle(33);
      break;
    case 'h':
      car.setAngle(48);
      break;
    case 'i':
      car.setAngle(60);
      break;
    //Left angles
    case 'j':
      car.setAngle(-33);
      break;
    case 'k':
      car.setAngle(-48);
      break;
    case 'l':
      car.setAngle(-60);
      break;
    //Break command
    case 'm':
      car.setSpeed(0);
      car.setAngle(0);
      break;
    case 'q':
      follow = true;
  }
}
void FollowLine() {
  currError = Error();
  float correction =  Kp * currError + Kd * (currError - lastError);
  lastError = currError;
  float rightMotorSpeed = MOTOR_BASE_SPEED + correction;
  float leftMotorSpeed = MOTOR_BASE_SPEED - correction;
  if (rightMotorSpeed > MOTOR_MAX_SPEED ) rightMotorSpeed = MOTOR_MAX_SPEED; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > MOTOR_MAX_SPEED ) leftMotorSpeed = MOTOR_MAX_SPEED; // prevent the motor from going beyond max speed
  if(rightMotorSpeed<0) rightMotorSpeed=0;
  if(leftMotorSpeed<0)leftMotorSpeed=0;
  car.setMotorSpeed(leftMotorSpeed,rightMotorSpeed); 
  delay(6); 
}
float Error(){
  float a=analogRead(A11),b=analogRead(A8),c=analogRead(A9),d=analogRead(A10), e=analogRead(A12);
  if(a>b && a>c && a>d && a>e)
  return leftout=(leftout +2.7);
  
  if(b>a && b>c && b>d && b>e){
  return 0;//reset all
  leftin=0,rightin=0,leftout=0,rightout=0;
  }
  if(c>a && c>b && c>d && c>e)
  return rightin=rightin-1.5;
  if(d>a && d>b && d>c && d>e)
  return rightout=(rightout -2.7);
  return leftin+=1.5;
}
