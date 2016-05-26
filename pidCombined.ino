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
long sensors_average;
int sensors_sum;
int position;
int set_point=2000;
int proportional, integral, derivative,last_proportional;
int error_value;
long sensors[] = {0, 0, 0, 0, 0};   // Array used to store 5 readings for the 5sensors.
int Kp=5, Ki=0.001, Kd=50;//tuned by testing 
int  right_speed=0, left_speed=0;
int  max_speed=90;
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
  proportional=0, integral=0, derivative=0,last_proportional=0;
  error_value=0;
}


void loop() {
  if (Serial3.available()) {
    in = Serial3.read();
    handleInput();
  }
  
  if(follow)
  {
     followLine();//notice the execution will stop here until we send the unfollow command "Q"
     car.setSpeed(0);//then we're stopping the car
     car.setAngle(0);
  }
   currentSpeed = (encoderRight.getSpeed() + encoderLeft.getSpeed())/2;
   
  Serial3.println("s");
  if(currentSpeed < 0.2){
    currentSpeed = 0;
    Serial3.println(currentSpeed);
  }else{
  Serial3.println(currentSpeed);
  }
  dis = encoderRight.getDistance();
  Serial3.println("d");
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
      if(follow == false){
        follow = true;
      }
      else if(follow == true){
        follow = false;
      }
      else{
        follow = false;
      }
  }
}
void followLine() 
{
  while(follow)
{
  read();//get the current position 
  pid();//get the correction vaule 
  pidTurn();//tune the correction vaule
  car.setMotorSpeed(right_speed, left_speed);//apply correction
  in = Serial3.read();
  if(in='q')
  follow = !follow;
  delay(3);
}  
}
void read()
{
sensors_average = 0;
sensors_sum = 0;
 
for (int i = 0; i < 5; i++){
if(i==0)  sensors[i] = analogRead(A11);//sensors from left to right (0,1,2,3,4)
if(i==1)  sensors[i] = analogRead(A12);
if(i==2)  sensors[i] = analogRead(A8);
if(i==3)  sensors[i] = analogRead(A9);
if(i==4)  sensors[i] = analogRead(A10);
sensors_average += sensors[i] * i * 1000;   //Calculating the weighted mean
sensors_sum += int(sensors[i]);}            //Calculating sum of sensor readings
}
void pid()
{
position = int(sensors_average / sensors_sum);
proportional = position - set_point;
integral = integral + proportional;
derivative = proportional - last_proportional;
last_proportional = proportional;
 
error_value = int(proportional * Kp + integral * Ki + derivative * Kd);
}
void pidTurn()
{  //Restricting the error value between +256.
if (error_value< -256)
{ error_value = -256;
} 
if (error_value> 256)
{
error_value = 256;
}
//turning the car left or right depening of the err vaule wether it's <0 or >0  
if (error_value< 0)
{
right_speed = max_speed + error_value;
left_speed = max_speed;
}
else
{
right_speed = max_speed;
left_speed = max_speed - error_value;
}
}
