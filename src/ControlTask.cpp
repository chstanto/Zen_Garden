/** @file ControlTask.cpp
 *  This file contains a task for controlling the Zen Gardener's magnet position
 *  @author  Aaron Tran
 *  @date    2021-Nov-29 Original file
 */


#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h" 

#include "ControlTask.h"
#include "Control.h"
#include "EncoderDriver.h"
#include "MotorDriver.h"
#include "EMDriver.h"
#include "math.h"

//Motor Pins
#define inputA1 PB8
#define inputA2 PB9
#define enableA PB6

#define inputB1 PA8
#define inputB2 PA9
#define enableB PB7

//Encoder Pins
#define E1CHA PA0
#define E1CHB PA1

#define E2CHA PB4
#define E2CHB PB5

//Electromagnet Pin
#define MagPin PA10

//Limit Switch Pins
#define limx PA4
#define limy PB0

//Driver setups
MotorDriver xMOT (inputA1, inputA2, enableA);
MotorDriver yMOT (inputB1, inputB2, enableB);
STM32Encoder xENC (TIM5, E1CHA, E1CHB);
STM32Encoder yENC (TIM3, E2CHA, E2CHB);
Control xCONT (1.003, 0.0203);
Control yCONT (1.003, 0.0203);
EMDriver mag (MagPin);

bool xNOThome = true; 
bool yNOThome = true;

void lim_switchx()
{
    xNOThome = false;
    xENC.zero();
}

void lim_switchy()
{
    yNOThome = false;
    yENC.zero();
}

void task_control(void* p_params)
{
    //Setup Code
    (void)p_params;                             // Shuts up a compiler warning
    uint8_t state = 0;
    //Position and velocity declarations
    float x_pos = 0;
    float y_pos = 0;
    float x_vel = 0;
    float y_vel = 0;
    float x_last = 0;
    float y_last = 0;
    float x_ref = 0;
    float y_ref = 0;
    float xdot_ref = 0;
    float ydot_ref = 0;
    float speed = 0.1; //Desired operating speed, [in/s]
    float x_dist = 0;
    float y_dist = 0;
    float ang = 0;
    unsigned long last_time = 0;
    
    pinMode(limx, INPUT);
    pinMode(limy, INPUT);
    attachInterrupt(limx, lim_switchx, RISING);
    attachInterrupt(limy, lim_switchy, RISING);    

    bool firstx = false;
    bool firsty = false;
    for(;;)
    {
        unsigned long Startime = micros();
        mag.enable();
        if(state ==0) //INIT State, find home setup everything
        {
            if (xNOThome)
            {
                xMOT.set_duty(-15); 
            }
            else
            {
                xMOT.Disable_MOT();
                detachInterrupt(digitalPinToInterrupt(limx));    //Detach interrupts to avoid accidentally rezeroing            
            }
            if (yNOThome)
            {
                yMOT.set_duty(-15);
                detachInterrupt(digitalPinToInterrupt(limy));    //Detach interrupts to avoid accidentally rezeroing    
            }
            else
            {
                yMOT.Disable_MOT();
            }
            if (xNOThome & yNOThome)
            {}
            else
            {
                xMOT.set_duty(15);
                yMOT.set_duty(15);
                x_ref = xref.get();
                y_ref = yref.get();
                state = 1;
            }
        }
        else if(state ==1)
        {
            //Prep to first position
            x_pos = xENC.update()*1.571/4000;
            y_pos = yENC.update()*1.571/4000;           
            if (x_pos >= x_ref)
            {
                xMOT.Disable_MOT();
            }
            if (y_pos >= y_ref)
            {
                yMOT.Disable_MOT();
            }
            if (firstx & firsty)
            {
                state = 2;
            }
        }
        else if(state ==2)
        {   
            //Read current values
            x_pos = xENC.update()*1.571/4000;
            y_pos = yENC.update()*1.571/4000;
            x_vel = (x_pos - x_last)/(micros()-last_time)*1000000;
            y_vel = (y_pos - y_last)/(micros()-last_time)*1000000;
            x_last = x_pos;
            y_last = y_pos;
            last_time = micros();
            x_ref = xref.get();
            y_ref = yref.get();

            //Determine reference velocities
            x_dist = x_ref - x_pos;
            y_dist = y_ref - y_pos;
            ang = atan2(y_dist, x_dist);

            xdot_ref = speed*cos(ang);
            ydot_ref = speed*sin(ang);

            xCONT.run(x_ref, x_pos, xdot_ref, x_vel);
            yCONT.run(y_ref, x_pos, ydot_ref, x_vel);

            if (data_NOTavail.get())
            {
                state = 3;
            }
        }
        else if(state ==3)
        {
            mag.disable();
        }
        vTaskDelay(20);
    }
}
