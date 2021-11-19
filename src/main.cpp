#include <Arduino.h>
#include <PrintStream.h>
#include "EncoderDriver.h"
#include "MotorDriver.h"

#define inputA1 PB8
#define inputA2 PB9
#define enableA PA6
#define limx PA4

static bool val = false;


void lim_switch()
{
  val = digitalRead(limx);
}


void setup()
{
  pinMode(limx, INPUT);
  attachInterrupt(limx, lim_switch, CHANGE);
  Serial<< "begin"<<endl;
  Serial.begin(115200);
  for(;;)
  {
  Serial<< val<< endl;
  delay(1000);
  }


}

void loop()
{
}







/*

void setup() 
{ 
  Serial.begin (115200);
  Serial <<"Initialized" << endl;

  MotorDriver mot1 (inputA1, inputA2, enableA);


  Serial <<"Sending duty" << endl;
  mot1.set_duty(50);

  Serial <<"wait 2.5 sec" << endl;
  delay(2500);

  Serial <<"Reverse duty" << endl;
  mot1.set_duty(-50);

  
  Serial <<"wait 2.5 sec" << endl;
  delay(2500);

  
  Serial <<"Turn off" << endl;
  mot1.set_duty(0);

}

void loop()
{

}



//Motor B
#define motorBpin1 PA9 //IN3
#define motorBpin2 PA8 //IN4
#define ENB PA5 //ENB

//Motor A
#define motorApin1 PB8 //IN1
#define motorApin2 PB9 //IN2
#define ENA PA6 //ENA

void setup() {
  Serial.begin (115200);
  Serial <<"Initialized" << endl;
  // put your setup code here, to run once:

  pinMode(motorApin1, OUTPUT); //Motor A
  pinMode(motorApin2, OUTPUT); //Motor A

  pinMode(ENA, OUTPUT); //Motor A
  pinMode(ENB, OUTPUT); //Motor B


}

void loop() {
  // put your main code here, to run repeatedly:   
  Serial << "no dont turn me into a hat"<< endl;
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(ENA, 255); //ENA pin

  //Controlling spin direction of motors:
  
  //Motor A (REV: CW)
  digitalWrite(motorApin1, HIGH);
  digitalWrite(motorApin2, LOW);
  delay(1000);
  
  //Motor A (REV)
  digitalWrite(motorApin1, LOW);
  digitalWrite(motorApin2, HIGH);
  delay(1000);

}*/