#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "EncoderDriver.h"
#include "MotorDriver.h"
#include "EMDriver.h"

//Pin definition
/*
//Motor Pins
#define inputA1 PB8
#define inputA2 PB9
#define enableA PA6

#define inputB1 PA8
#define inputB2 PA9
#define enableB PA5

//Encoder Pins
#define E1CHA PA0
#define E1CHB PA1

#define E2CHA PB4
#define E2CHB PB5

//Limit Switch Pins
#define limx PA4
#define limy PB0

static bool xzero = false;
static bool yzero = false;

//Electromagnet Pin
#define MagPin PA10
*/

//Task Scheduler
/*
void setup () 
{
    // Start the serial port, wait a short time, then say hello. Use the
    // non-RTOS delay() function because the RTOS hasn't been started yet
    Serial.begin (115200);
    delay (2000);
    Serial << endl << endl << "Hello, I am an RTOS demonstration" << endl;

    // Create a task which prints a slightly disagreeable message
    xTaskCreate (task_MOTx,
                 "x_motor",                         // Task name for printouts
                 2048,                            // Stack size
                 NULL,                            // Parameters for task fn.
                 4,                               // Priority
                 NULL);                           // Task handle

    // Create a task which prints a more agreeable message
    xTaskCreate (task_MOTy,
                 "y_motor",
                 2048,                            // Stack size
                 NULL,
                 4,                               // High priority
                 NULL);

    // Create a task which prints a more agreeable message
    xTaskCreate (task_EM,
                 "EM",
                 2048,                            // Stack size
                 NULL,
                 5,                               // Priority
                 NULL);

    // Create a task which prints a more agreeable message
    xTaskCreate (task_data,
                 "data",
                 4096,                            // Stack size
                 NULL,
                 1,                               // Priority
                 NULL);

    // Create a task which prints a more agreeable message
    xTaskCreate (task_control,
                 "control",
                 2048,                            // Stack size
                 NULL,
                 1,                               // Priority
                 NULL);

    // If using an STM32, we need to call the scheduler startup function now;
    // if using an ESP32, it has already been called for us
    #if (defined STM32L4xx || defined STM32F4xx)
        vTaskStartScheduler ();
    #endif
}

void loop()
{

}
*/

//EM Testing
/*
void setup()
{  
  Serial.begin (115200);
  delay(5000);

  Serial <<"Initialized" << endl;
  EMDriver mag (MagPin);
  mag.enable();
  delay(10000);
  Serial <<"Disabling" << endl;
  mag.disable();
}

void loop()
{

}
*/

//Limit Switch Testing
/*
void lim_switchx()
{
  xzero = digitalRead(limx);
  enc1.zero();
}

void lim_switchy()
{
  yzero = digitalRead(limx);
  enc2.zero();
}

void setup() 
{ 
  //Setup limit switches
  pinMode(limx, INPUT);
  pinMode(limy, INPUT);
  attachInterrupt(limx, lim_switchx, CHANGE);
  attachInterrupt(limy, lim_switchy, CHANGE);
} 

void loop()
{
}
*/

//Encoder Testing
/*
void setup()
{ 
  //Test
  Serial.begin (115200);
  delay(5000);

  //Setup encoders

  Serial <<"Setting up encoder 1!" << endl;
  STM32Encoder enc1 (TIM5, E1CHA, E1CHB);
  Serial <<"Setting up encoder 2!" << endl;
  STM32Encoder enc2 (TIM3, E2CHA, E2CHB);

  for(;;)
  {
  delay(1000);  
  Serial <<"Motor 1 Position:" << enc1.getCount()*360/4000 << "deg" << endl;
  Serial <<"Motor 2 Position:" << enc2.getCount()*360/4000 << "deg" << endl;
  }
}

void loop()
{
}
*/

//Motor + Encoder Testing
/*
void setup() 
{ 
  //Setup motors
  MotorDriver mot1 (inputA1, inputA2, enableA);
  MotorDriver mot2 (inputB1, inputB2, enableB);
  

  //Test
  Serial.begin (115200);
  delay(5000);

  Serial <<"Initialized" << endl;
  int PWM = 100;

  // Setup encoders
  STM32Encoder enc1 (TIM2, E1CHA, E1CHB);
  Serial <<"Do something!" << endl;
  STM32Encoder enc2 (TIM3, E2CHA, E2CHB);

  Serial <<"Sending duty" << endl;
  mot1.set_duty(PWM);
  mot2.set_duty(PWM);

  Serial <<"wait 2.5 sec" << endl;
  delay(2500);
  Serial <<"Motor 1 Position:" << enc1.getCount()*360/4000 << "deg" << endl;
  Serial <<"Motor 2 Position:" << enc2.getCount()*360/4000 << "deg" << endl;


  Serial <<"Reverse duty" << endl;
  mot1.set_duty(-PWM);
  mot2.set_duty(-PWM);
  
  Serial <<"wait 2.5 sec" << endl;
  delay(2500);
  Serial <<"Motor 1 Position:" << enc1.getCount()*360/4000 << "deg" << endl;
  Serial <<"Motor 2 Position:" << enc2.getCount()*360/4000 << "deg" << endl;


  Serial <<"Reverse again" << endl;
  mot1.set_duty(PWM);
  mot2.set_duty(PWM);

  Serial <<"wait 2.5 sec" << endl;
  delay(2500);
  Serial <<"Motor 1 Position:" << enc1.getCount()*360/4000 << "deg" << endl;
  Serial <<"Motor 2 Position:" << enc2.getCount()*360/4000 << "deg" << endl;


  Serial <<"Turn off motor 1" << endl;
  mot1.Disable_MOT();
  Serial <<"Turn off motor 2" << endl;
  mot2.Disable_MOT();
  Serial <<"All motors disabled" << endl;

}

void loop()
{
}
*/

//Internet Motor Code
/*
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
  

  pinMode(motorBpin1, OUTPUT); //Motor A
  pinMode(motorBpin2, OUTPUT); //Motor A
  
  pinMode(ENA, OUTPUT); //Motor A
  pinMode(ENB, OUTPUT); //Motor B


}

void loop() {
  // put your main code here, to run repeatedly:   
  Serial << "no dont turn me into a hat"<< endl;
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(ENA, 255); //ENA pin
  analogWrite(ENB, 255); //ENA pin

  //Controlling spin direction of motors:
  
  //Motor A (REV: CW)
  digitalWrite(motorApin1, HIGH);
  digitalWrite(motorApin2, LOW);
  delay(1000);
  
  //Motor A (REV)
  digitalWrite(motorApin1, LOW);
  digitalWrite(motorApin2, HIGH);
  delay(1000);

  //Motor A (REV: CW)
  digitalWrite(motorBpin1, HIGH);
  digitalWrite(motorBpin2, LOW);
  delay(1000);
  
  //Motor A (REV)
  digitalWrite(motorBpin1, LOW);
  digitalWrite(motorBpin2, HIGH);
  delay(1000);



}
*/
