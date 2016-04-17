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
      speed = 40;
      car.setSpeed(speed);
      break;
    case 'b':
      speed = 70;
      car.setSpeed(speed);
      break;
    case 'c':
      speed = 100;
      car.setSpeed(speed);
      break;
    //Backward speeds
    case 'd':
      speed = -40;
      car.setSpeed(speed);
      break;
    case 'e':
      speed = -70;
      car.setSpeed(speed);
      break;
    case 'f':
      speed = -100;
      car.setSpeed(speed);
      break;
    //Right angles
    case 'g':
      car.setAngle(10);
      break;
    case 'h':
      car.setAngle(30);
      break;
    case 'i':
      car.setAngle(60);
      break;
    //Left angles
    case 'j':
      car.setAngle(-10);
      break;
    case 'k':
      car.setAngle(-30);
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
  while(true){
   in = Serial3.read();
   if(in=='m')
   break;
  if(greatest().equals("CENTER"))
  car.setMotorSpeed(50,50);
  if(greatest().equals("LEFT IN"))
  car.setMotorSpeed(0,50);
  if(greatest().equals("LEFT OUT"))
  car.setMotorSpeed(0,50);
  if(greatest().equals("RIGHT IN"))
  car.setMotorSpeed(50,0);
  if(greatest().equals("RIGHT OUT"))
  car.setMotorSpeed(50,0);
  delay(50);
  }
}

String greatest(){
  int a=analogRead(A11),b=analogRead(A8),c=analogRead(A9),d=analogRead(A10), e=analogRead(A12);
  if(a>b && a>c && a>d && a>e)
  return "LEFT OUT";
  if(b>a && b>c && b>d && b>e)
  return "CENTER";
  if(c>a && c>b && c>d && c>e)
  return "RIGHT IN";
  if(d>a && d>b && d>c && d>e)
  return "RIGHT OUT";
  return "LEFT IN";
}

