#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif

#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues
#include "shares.h"            // Header for shares used in this project

#include "EncoderDriver.h"
#include "MotorDriver.h"
#include "EMDriver.h"

/*
#include "ControlTask.h"
#include "TestTask.h"

Queue<float> xref(30, "Data");
Queue<float> yref(30, "Data");
Queue<uint8_t> data_NOTavail(30, "Data");
*/

//Pin definition

//Motor Pins
#define inputA1 PB8
#define inputA2 PB9
#define REALenableA PB6

#define inputB1 PA8
#define inputB2 PA9
#define REALenableB PB7

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

//Control Task Testing
/*
void setup()
{


  xTaskCreate (task_control,
               "control",
               2048,                            // Stack size
               NULL,
               1,                               // Priority
               NULL);
  xTaskCreate (task_test,
               "send data",
               2048,                            // Stack size
               NULL,
               1,                               // Priority
               NULL);
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
  delay(30000);
  Serial <<"Disabling" << endl;
  mag.disable();
}

void loop()
{

}
*/

//Limit Switch Testing


void setup() 
{ 
  Serial.begin (115200);
  delay(5000);
  //Setup limit switches
  pinMode(limx, INPUT);
  pinMode(limy, INPUT);
  
  //attachInterrupt(limx, lim_switchx, FALLING);
  //attachInterrupt(limy, lim_switchy, FALLING);
  
  for(;;)
  {
  delay(500);
  Serial << "limx is: " << xzero << endl;
  Serial << "limy is: " << yzero << endl;    
  }
} 

void loop()
{

}

void lim_switchx()
{
  Serial << "called lim_switchx()"<< endl;
  xzero = true;
  //enc1.zero();
}

void   lim_switchy()
{
  Serial << "called lim_switchy()"<< endl;
  yzero = true;
  //enc2.zero();
}


//Encoder Testing
/*
void setup()
{ 
  //Test
  Serial.begin (115200);
  delay(5000);

  //Setup encoders

  Serial <<"Setting up encoder 1!" << endl;
  STM32Encoder enc1 (TIM2, E1CHA, E1CHB);
  Serial <<"Setting up encoder 2!" << endl;
  STM32Encoder enc2 (TIM3, E2CHA, E2CHB);

  enc1.zero();
  enc2.zero();

  float x_pos;
  float y_pos;

  for(;;)
  {
  delay(2500);  
  x_pos = enc1.update()*1.571/4000;
  y_pos = enc2.update()*1.571/4000;
  Serial <<"Motor 1 Position:" << x_pos << "inches" << endl;
  Serial <<"Motor 2 Position:" << y_pos << "inches" << endl;
  //Serial <<"Motor 1 Position:" << enc1.update()*360/4000 << "degrees" << endl;
  //Serial <<"Motor 2 Position:" << enc2.update()*360/4000 << "degrees" << endl;
  //Serial <<"Motor 1 delta:" << enc1.update() << "ticks" << endl;
  //Serial <<"Motor 1 Position:" << enc1.delta << "ticks" << endl;
  //Serial <<"Motor 2 delta:" << enc2.update() << "ticks" << endl;
  //Serial <<"Motor 2 Position:" << enc2.delta << "ticks" << endl;
  }
}

void loop()
{
}
*/

//Motor Testing
/*
void setup() 
{ 
  //Setup motors
  MotorDriver mot1 (inputA1, inputA2, REALenableA);
  MotorDriver mot2 (inputB1, inputB2, REALenableB);
  
  //Test
  Serial.begin (115200);
  delay(5000);
  
  Serial <<"Initialized" << endl;
  int PWM = -15;
  int delay_val = 500;

  Serial <<"Sending duty" << endl;
  mot1.set_duty(PWM);
  mot2.set_duty(PWM);
  delay(delay_val);

  //Disable
  mot1.Disable_MOT();
  mot2.Disable_MOT();
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
  MotorDriver mot1 (inputA1, inputA2, REALenableA);
  MotorDriver mot2 (inputB1, inputB2, REALenableB);
  

  //Test
  Serial.begin (115200);
  delay(5000);
  
  Serial <<"Initialized" << endl;
  int PWM = 20;
  int delay_val = 50;


  // Setup encoders
  STM32Encoder enc1 (TIM2, E1CHA, E1CHB);
  Serial <<"Do something!" << endl;
  STM32Encoder enc2 (TIM3, E2CHA, E2CHB);

  enc1.zero();
  enc2.zero();

  Serial <<"Sending duty" << endl;
  //mot1.set_duty(PWM);
  mot2.set_duty(PWM);

  float x_pos = -enc1.update()*2*3.14159/4000*0.24;
  float y_pos = -enc2.update()*2*3.14159/4000*0.24;

  Serial <<"Motor 1 Position:" << x_pos << "inches" << endl;
  Serial <<"Motor 2 Position:" << y_pos << "inches" << endl;

  delay(2500);
  x_pos = -enc1.update()*2*3.14159/4000*0.24;
  y_pos = -enc2.update()*2*3.14159/4000*0.24;
  Serial <<"Motor 1 Position:" << x_pos << "inches" << endl;
  Serial <<"Motor 2 Position:" << y_pos << "inches" << endl;  
  mot1.Disable_MOT();
  mot2.Disable_MOT();
}

void loop()
{
}
*/

//Internet Motor Code
/*
//Motor B
#define motorBpin1 PA8 //IN3
#define motorBpin2 PA9 //IN4
#define realENB PB7 //ENB
#define fakeENB PA5

//Motor A
#define motorApin1 PB8 //IN1
#define motorApin2 PB9 //IN2
#define realENA PB6 //ENA
#define fakeENA PA6


void setup() {
  Serial.begin (115200);
  delay(5000);
  Serial <<"Initialized" << endl;
  // put your setup code here, to run once:

  pinMode(motorApin1, OUTPUT); //Motor A
  pinMode(motorApin2, OUTPUT); //Motor A
  

  pinMode(motorBpin1, OUTPUT); //Motor A
  pinMode(motorBpin2, OUTPUT); //Motor A
  

  ////pinMode(fakeENA, INPUT); //Motor A
  pinMode(realENA, OUTPUT); //Motor A
  
  ////pinMode(fakeENB, INPUT); //Motor A  
  pinMode(realENB, OUTPUT); //Motor B

  // put your main code here, to run repeatedly:   
  Serial << "no dont turn me into a hat"<< endl;
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(realENA, 0.25*255); //ENA pin
  analogWrite(realENB, 0.25*255); //ENA pin

  Serial << analogRead(realENB) << endl;
  
  //Controlling spin direction of motors:
  
  //Motor A (REV: CW)
  digitalWrite(motorApin1, HIGH);
  digitalWrite(motorApin2, LOW);

  //Motor B (REV: CW)
  digitalWrite(motorBpin1, HIGH);
  digitalWrite(motorBpin2, LOW);
  delay(100);

  //Disable
  analogWrite(realENA, 0*255); //ENA pin
  analogWrite(realENB, 0*255); //ENA pin

}

void loop() {
{

}
*/

