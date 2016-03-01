#include <Smartcar.h>
/*
 * Full Serial control via bluetooth, the car will only move by rotating it after encoutering an obstacle 
 */
Car car;
Odometer encoderLeft, encoderRight;
Gyroscope gyro;
SR04 ultrasonicSensor1;
SR04 ultrasonicSensor2;
int speed,front,back, angle;
char in;
int dis;
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
  ultrasonicSensor2.attach(6,5);
  in=0;
  speed=0;
  //
  front=0;
  back=0;
  angle=0;
  dis = 0;
  currentSpeed = 0;
}


void loop() {
if(Serial3.available()){
in=Serial3.read();
handleInput();

}else{
if(frontIsClear()==false){  
speed = 0;
angle = 0;
car.setAngle(angle);
car.setSpeed(speed);
}
}
  
}

void handleInput() { //handle serial input if there is any
  currentSpeed = encoderRight.getSpeed();
  Serial3.print("s");
  Serial3.println(currentSpeed);
  dis = encoderRight.getDistance();
  Serial3.print("d");
  Serial3.println(dis/100);
  
  
  switch(in){
  case 'w': if(speed<100 && speed !=0)
               speed = speed + 10;
               if(angle<0)
                 angle = angle + 10;
                 if(angle>0)
                   angle = angle - 10;
                   if(speed == 0)
                     speed = 40;

            car.setAngle(angle);
            car.setSpeed(speed);
            
                 
   
    break;

  case 's': if(speed>-100 && speed!=0)
               speed = speed - 10;
               if(angle>0)
                  angle = angle - 10;
                  if(angle<0)
                     angle = angle + 10;
                    if(speed == 0)
                       speed = -40;
            car.setAngle(angle);         
            car.setSpeed(speed);  
            

    break;   

  case 'd': if(angle<60)
               angle = angle + 20;
              
            car.setAngle(angle);
            car.setSpeed(speed);
  
    break;

  case 'a':  if(angle>-60)
               angle = angle - 20;
              
            car.setAngle(angle);
            car.setSpeed(speed);

    break;

 case 'q': car.rotate(-80);

 break;

 case 'e': car.rotate(80);
 break;
  
case 'f': speed = 0;
         angle = 0;
         car.setAngle(angle);
         car.setSpeed(speed);
         
  }
if(frontIsClear()==false){  
speed = 0;
angle = 0;
car.setAngle(angle);
car.setSpeed(speed);
}
}

boolean frontIsClear(){
 front =  ultrasonicSensor2.getDistance();
 if(front>20)// change the distance here (since the ultra sonic is far from being ideal)
 return true;
 if(front == 0)
 return true;

 return false;
}

//int backIsClear(){
//back = ultrasonicSensor1.getDistance();
//return back;  
//}
