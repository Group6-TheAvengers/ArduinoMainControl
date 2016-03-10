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
}


void loop() {
  if (Serial3.available()) {
    in = Serial3.read();
    handleInput();

  }
}

void handleInput() { //handle serial input if there is any
  currentSpeed = encoderRight.getSpeed();
  Serial3.print("s");
  Serial3.println(currentSpeed);
  dis = encoderRight.getDistance();
  Serial3.print("d");
  Serial3.println(dis/100);

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
  }
}

boolean frontIsClear() {
  front =  ultrasonicSensor2.getDistance();
  if (front > 20) // change the distance here (since the ultra sonic is far from being ideal)
    return true;
  if (front == 0)
    return true;

  return false;
}
