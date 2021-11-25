#include <Arduino.h>
#include <PrintStream.h>
#include <MotorDriver.h>

MotorDriver::MotorDriver(uint8_t motP1, uint8_t motP2, uint8_t ENP)
{
  motorpin1 = motP1;
  motorpin2 = motP2;

  EN = ENP;

  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT); 
  pinMode(EN, OUTPUT);

}


void MotorDriver::Enable_MOT(void)
{
  analogWrite(EN, last_duty);
}

void MotorDriver::Disable_MOT(void)
{
  analogWrite(EN, 0);
}

void MotorDriver::set_duty (float duty)
{
  last_duty = duty*255/100;
  if (duty < 0)
  {
    digitalWrite(motorpin1, HIGH);
    digitalWrite(motorpin2, LOW);
    duty *= -1;
  }
  else
  {
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, HIGH); 
  };
  analogWrite(EN, duty*255/100);
  
}


/*
//Motor B
#define motorBpin1 PA9 //IN3
#define motorBpin2 PA8 //IN4
#define ENB PA6 //ENB

//Motor A
#define motorApin1 PB8 //IN1
#define motorApin2 PB9 //IN2
#define ENA PA5 //ENA

void setup() {
  Serial.begin (115200);
  Serial <<"Initialized" << endl;
  // put your setup code here, to run once:
  pinMode(motorBpin1, OUTPUT); //Motor B
  pinMode(motorBpin2, OUTPUT); //Motor B
  pinMode(motorApin1, OUTPUT); //Motor A
  pinMode(motorApin2, OUTPUT); //Motor A

  pinMode(ENA, OUTPUT); //Motor A
  pinMode(ENB, OUTPUT); //Motor B


}

void loop() {
  // put your main code here, to run repeatedly:   
  Serial << "no dont turn me into a hat"<< endl;
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(ENA, 0); //ENA pin
  analogWrite(ENB, 0); //ENB pin

  //Controlling spin direction of motors:
  
  //Motor A (REV: CW)
  digitalWrite(motorApin1, HIGH);
  digitalWrite(motorApin2, LOW);
  delay(1000);

  //Motor B (FWD: CW)
  digitalWrite(motorBpin1, HIGH);   
  digitalWrite(motorBpin2, LOW);
  
  //Motor B (FWD)
  digitalWrite(motorBpin1, LOW);
  digitalWrite(motorBpin2, HIGH);

  //Motor A (REV)
  digitalWrite(motorApin1, LOW);
  digitalWrite(motorApin2, HIGH);
  delay(1000);

}*/