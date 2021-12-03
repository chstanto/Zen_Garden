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
#define limx PB0
#define limy PA4

//Driver setups
MotorDriver yMOT (inputA1, inputA2, enableA);
MotorDriver xMOT (inputB1, inputB2, enableB);
STM32Encoder yENC (TIM2, E1CHA, E1CHB);
STM32Encoder xENC (TIM3, E2CHA, E2CHB);
Control xCONT (0.00, 0.4); //0.004
Control yCONT (0.00, 0.75); //0.5 is too high
EMDriver mag (MagPin);

void task_control(void* p_params)
{
    Serial.begin (115200);
    delay(5000);
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
    float x_err = 0;
    float y_err = 0;
    float xdot_ref = 0;
    float ydot_ref = 0;
    float speed = 0.01; //Desired operating speed, [in/s]
    float x_dist = 0;
    float y_dist = 0;
    float ang = 0;

    unsigned long last_time = 0;
    uint8_t flag;
    uint8_t delay_val = 10;
    
    pinMode(limx, INPUT_PULLUP);
    pinMode(limy, INPUT_PULLUP);

    uint8_t xNOThome = digitalRead(limx);
    uint8_t yNOThome = digitalRead(limy);

    bool firstx = false;
    bool firsty = false;
    bool x_ok = false;
    bool y_ok = false;
    for(;;)
    {
        unsigned long Startime = micros();
        mag.enable();
        if(state ==0) //INIT State, find home setup everything
        {
            xNOThome = digitalRead(limx);
            yNOThome = digitalRead(limy);
            if (xNOThome)
            {
                xMOT.set_duty(-25); 
            }
            else
            {
                xENC.zero();
                xMOT.Disable_MOT();
            }
            if (yNOThome)
            {
                yMOT.set_duty(-25);
            }
            else
            {
                yENC.zero();
                yMOT.Disable_MOT();
            }
            if (xNOThome | yNOThome)
            {
                Serial << "You are in state " << state << endl;
                Serial << "xNOThome is " << xNOThome << endl;
                Serial << "yNOThome is " << yNOThome << endl;
            }
            else
            {
                xMOT.set_duty(20);
                yMOT.set_duty(22);
                x_ref = xref.get();
                y_ref = yref.get();
                state = 1;
            }
        }
        else if(state ==1)
        {
            //Prep to first position
            x_pos = -xENC.update()*1.571/4000;
            y_pos = -yENC.update()*1.571/4000;           
            if (x_pos >= x_ref)
            {
                firstx = true;
                xMOT.Disable_MOT();
            }
            if (y_pos >= y_ref)
            {
                firsty = true;
                yMOT.Disable_MOT();
            }

            if (firstx & firsty)
            {
                state = 2;
                Serial << "Transitioning to state 2" << endl;
                Serial << "x position:" << x_pos << endl;
                Serial << "y position:" << y_pos << endl << endl;
            }
            else
            {
                Serial << "You are in state " << state << endl;
                Serial << "x position:" << x_pos << endl;
                Serial << "y position:" << y_pos << endl << endl;
            }
        }
        else if(state ==2)
        {   
            //Read current values
            x_pos = -xENC.update()*1.571/4000;
            y_pos = -yENC.update()*1.571/4000;
            x_vel = (x_pos - x_last)/(micros()-last_time)*1000000;
            y_vel = (y_pos - y_last)/(micros()-last_time)*1000000;
            x_last = x_pos;
            y_last = y_pos;
            last_time = micros();
            x_err = x_ref - x_pos;
            y_err = y_ref - y_pos;
                if( abs(x_err)<0.1)
                {
                    x_ok = true;
                    //xMOT.Disable_MOT();
                }
                else
                {
                    //xCONT.run(x_ref, x_pos, 0, x_vel);
                    //xMOT.set_duty(xCONT.PWM);
                }
                if( abs(y_err)<0.1)
                {
                    y_ok = true;
                    //yMOT.Disable_MOT();
                }
                else
                {
                    //yCONT.run(y_ref, y_pos, 0, y_vel);
                    //yMOT.set_duty(yCONT.PWM);
                }
                if (x_ok & y_ok)
                {
                    x_ref = xref.get();
                    y_ref = yref.get();
                    flag = data_NOTavail.get();
                    x_ok = false;
                    y_ok = false;
                }

            xCONT.run(x_ref, x_pos, 0, x_vel);
            xMOT.set_duty(xCONT.PWM);
            yCONT.run(y_ref, y_pos, 0, y_vel);
            yMOT.set_duty(yCONT.PWM);            
            //Determine reference velocities
            x_dist = x_ref - x_pos;
            y_dist = y_ref - y_pos;
            ang = atan2(y_dist, x_dist);

            xdot_ref = speed*cos(ang);
            ydot_ref = speed*sin(ang);

            if (flag)
            {
                xMOT.Disable_MOT();
                yMOT.Disable_MOT();
                state = 3;
            }
            Serial << "You are in state " << state << endl;
            //Serial << "Data not available?: " << flag << endl;

            Serial << "x pos: " << x_pos << "[in]"<< endl;
            Serial << "y pos: " << y_pos << "[in]"<< endl;

            Serial << "x ref: " << x_ref << "[in]"<< endl;
            Serial << "y ref: " << y_ref << "[in]"<< endl;

            Serial << "x error: " << x_dist << "[in]"<< endl;
            Serial << "y error: " << y_dist << "[in]"<< endl << endl;
            
            Serial << "x vel: " << x_vel << "[in/s]"<< endl;
            Serial << "y vel: " << y_vel << "[in/s]"<< endl;

            Serial << "x vel ref: " << xdot_ref << "[in/s]"<< endl;
            Serial << "y vel ref: " << ydot_ref << "[in/s]"<< endl;

            //Serial << "x vel error: " << xdot_ref - x_vel << "[in/s]"<< endl;
            //Serial << "y vel error: " << ydot_ref - y_vel << "[in/s]"<< endl << endl;

            Serial << "xPWM: " << xCONT.PWM << "%" << endl;
            Serial << "yPWM: " << yCONT.PWM << "%" << endl 
            <<"-----------------------------------------------------------------------------------------------------" <<endl;

           

        }
        else if(state ==3)
        {
            mag.disable();
        }
        vTaskDelay(delay_val);
    }
}
