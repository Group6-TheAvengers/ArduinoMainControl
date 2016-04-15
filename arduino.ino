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
int right, center;
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
  right=0, center=0;// Line sensors give higher values for darker colours
  follow = false;
}


void loop() {
  updateValues();
  if (Serial3.available()) {
    in = Serial3.read();
    handleInput();
    if(follow == true) {
    linefollowing();
    }
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
      follow = false;
      break;
    case 'q':
      follow = true;
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

void updateValues(){//P.S I forgot the correct pins assosiated with the sensors
right = analogRead(A10);//put the correct pin here
center = analogRead(A8);//put the correct pin here  
}

void linefollowing() {
  updateValues();// read the values from the sensors

if(left>=center && left>right){//if the line is below the left sensor (it could be below both the left and the center one)
  car.setMotorSpeed(0,50);//rotate left
}else if(right>=center && left<right){//Same case but on the oppsite direction (right)
  car.setMotorSpeed(50,0);
}else if(center>left && center>right){//if the center sensor is the only one above the line just advance forward 
  car.setMotorSpeed(50,50);// go ahead
}
} //Otherwise if there's no black (or darker) line then just do nothing.



