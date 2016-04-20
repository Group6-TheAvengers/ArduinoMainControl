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
int right, lef;
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
  right=70, left=70;// Line sensors give higher values for darker colours
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

/*
 * return the sensor that's detecting the line
 */
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

/*
 * A simple sketch to follow a line...if the line is on the CENTER the car will just move forward 
 * if it's below LEFTIN or RIGHTIN the car will turn slighty towards the oppsite direction and if the line stays there the turning angle will increase 
 * the LEFTOUT and RIGHTOUT are more like safty triggers if the line goes below them the car will rotate slowly untill we get back LEFTIN/RIGHTIN
 */
void linefollowing() {
while(true){   
if(greatest().equals("CENTER")){
  car.setMotorSpeed(100,100);
  left=50;
  right=50;
  }
  if(greatest().equals("LEFT IN")){
    left=left-20;
  car.setMotorSpeed(left,100);
  }
  if(greatest().equals("LEFT OUT")){
    car.setMotorSpeed(0,0);
  car.setMotorSpeed(0,30);
  }
  if(greatest().equals("RIGHT IN")){
    right=right-20;
  car.setMotorSpeed(100,right);
  }
  if(greatest().equals("RIGHT OUT")){
    car.setMotorSpeed(0,0);
  car.setMotorSpeed(30,0);
  }
}
} //Otherwise if there's no black (or darker) line then just do nothing.



