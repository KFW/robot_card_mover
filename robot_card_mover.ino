/******************************************************************************
robot_card_mover
Motor runs based on time - limit switches didn't work because of momemntum
For now button on A2 (16) used as trigger- will be IR sensor

this version uses Seeed Studio motor shield

code based on examples from:
http://www.seeedstudio.com/wiki/Motor_Shield_V1.0


EnableA(B)  In1(3)  In2(4)  Motion
   0          x       x      stop
   1          0       0      stop
   1          0       1      clockwise
   1          1       0      counterclockwise
   1          1       1      stop (?? brake -- need to check that)  
   
Speed control is achieved through conventional PWM which can be obtained from Arduino's PWM 
output D9(EA) and D10(EB). Using the function analogWrite() to generate the drive PWM:
int speedpinA=9;//enable motor A
int speedpinB=10;//enable motor B
int speed =127;//define the speed of motor
analogWrite(speedpinA,speed);

will only need motor A
attach servos to D1 & D2 with adapter cable

trigger input on A0 (with adapter cable) 


******************************************************************************/
#include <Servo.h>
Servo wrist;
Servo claw;

int pinI1=8;       //define I1 interface (motor A)
int pinI2=11;      //define I2 interface 
int speedpinA=9;   //enable motor A
//int pinI3=12;      //define I3 interface 
//int pinI4=13;      //define I4 interface 
//int speedpinB=10;  //enable motor B
int speed = 255;     //define the spead of motor; always full speed for this

const int Trigger = 0; // A0 for IR sensor trigger
int triggerVal;        // for holding value of read
boolean triggerFlag = false;

void setup(){
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);
//  pinMode(pinI3,OUTPUT);
//  pinMode(pinI4,OUTPUT);
//  pinMode(speedpinB,OUTPUT);

  wrist.attach(1);
  claw.attach(2);  
  
} // end setup()


void loop(){
  triggerVal = analogRead(Trigger);
  if (triggerVal > 500) {
    triggerFlag = true;
  }
   
  if (triggerFlag == true) {
    triggerFlag = false; // reset flag
  
    forward();
    delay(6000);
    halt();
    delay(1000);
    wrist.write(0); // should be forward down position
    delay(500);  
    claw.write(10); // since pads in place don't close fully
    delay(500);
    for (int i = 0; i < 105; i++){ // see if moving all the way draws too much power
        wrist.write(i); // upright; overshoots @ 127
        delay(5);
    }
    delay(1000);
    backward();
    delay(6000);
    halt();
    delay(500);
    wrist.write(255);
//    for (int j = 90; j < 256; j++){
//      wrist.write(j); // should be back down position
//      delay(5);
//    }
    delay(500);
    claw.write(65);  // open 1/4 to release
    delay(500);
    for (int k = 255; k > 104; k--){
      wrist.write(k); // back upright
      delay(5);
    }    
    delay(1000); 
  } // end if
  
} // end loop()

/////////////////////// FUNCTIONS ///////////////////////

void forward(){
  analogWrite(speedpinA,speed);//input a simulation value to set the speed
  digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
  digitalWrite(pinI1,HIGH);
}

void backward(){
  analogWrite(speedpinA,speed);//input a simulation value to set the speed
  digitalWrite(pinI2,HIGH); //turn DC Motor A move clockwise
  digitalWrite(pinI1,LOW);
}

void halt(){
  digitalWrite(speedpinA,LOW); // Unenable the pin to stop the motor. this should be done to avid damaging the motor. 
  delay(10);
}

