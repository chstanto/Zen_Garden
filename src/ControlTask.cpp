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
Control xCONT (0.0015, 0.203*0.5); // 1.003, 0.0203
Control yCONT (0.0015, 0.203*0.5); //0.0203
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
    float xdot_ref = 0;
    float ydot_ref = 0;
    float speed = 0.1; //Desired operating speed, [in/s]
    float x_dist = 0;
    float y_dist = 0;
    float ang = 0;
    float x_acc = 0;
    float y_acc = 0;
    unsigned long last_time = 0;
    uint8_t flag;
    int delay_val = 100;
    
    pinMode(limx, INPUT_PULLUP);
    pinMode(limy, INPUT_PULLUP);

    uint8_t xNOThome = digitalRead(limx);
    uint8_t yNOThome = digitalRead(limy);

    bool firstx = false;
    bool firsty = false;
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
                xMOT.set_duty(14);
                yMOT.set_duty(16);
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
                delay_val = 100;
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
        /*
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

            xMOT.set_duty(xCONT.PWM);
            yMOT.set_duty(xCONT.PWM);


            flag = data_NOTavail.get();

            if (flag)
            {
                xMOT.Disable_MOT();
                yMOT.Disable_MOT();
                state = 3;
            }
            Serial << "You are in state " << state << endl;
            Serial << "Data not available?: " << flag << endl;

            Serial << "x pos: " << x_pos << "[in]"<< endl;
            Serial << "y pos: " << y_pos << "[in]"<< endl;

            Serial << "x ref: " << x_ref << "[in]"<< endl;
            Serial << "y ref: " << y_ref << "[in]"<< endl;

            Serial << "x error: " << x_dist << "[in]"<< endl;
            Serial << "y error: " << y_dist << "[in]"<< endl << endl;
            
            Serial << "x vel: " << x_vel << "[in]"<< endl;
            Serial << "y vel: " << y_vel << "[in]"<< endl;

            Serial << "x vel ref: " << xdot_ref << "[in]"<< endl;
            Serial << "y vel ref: " << ydot_ref << "[in]"<< endl;

            Serial << "x vel error: " << xdot_ref - x_vel << "[in]"<< endl;
            Serial << "y vel error: " << ydot_ref - y_vel << "[in]"<< endl << endl;

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
*/
        else if(state ==2)
        {   
            if(xref.get())
            {
                x_ref = xref.get();
                y_ref = yref.get();
                state = 3;
            }
            else
            {
                state = 4;
            }
        }
        else if(state == 3)
        {
            
            //Read current values
            x_pos = -xENC.update()*1.571/4000;
            y_pos = -yENC.update()*1.571/4000;
            x_vel = (x_pos - x_last)/(micros()-last_time)*1000000;
            y_vel = (y_pos - y_last)/(micros()-last_time)*1000000;
            x_last = x_pos;
            y_last = y_pos;
            last_time = micros();

            x_acc = x_pos/x_ref;
            y_acc = y_pos/y_ref;

            if(0.9 < x_acc < 1.1 & 0.9 < y_acc < 1.1)
            {
                state = 2;
            }
            else
            {
                //Determine reference velocities
                x_dist = x_ref - x_pos;
                y_dist = y_ref - y_pos;
                ang = atan2(y_dist, x_dist);

                xdot_ref = speed*cos(ang);
                ydot_ref = speed*sin(ang);

                xCONT.run(x_ref, x_pos, xdot_ref, x_vel);
                yCONT.run(y_ref, x_pos, ydot_ref, x_vel);

                xMOT.set_duty(xCONT.PWM);
                yMOT.set_duty(xCONT.PWM);

                /*
                flag = data_NOTavail.get();

                if (flag)
                {
                    xMOT.Disable_MOT();
                    yMOT.Disable_MOT();
                    state = 3;
                }
                */
                Serial << "You are in state " << state << endl;
                Serial << "Data not available?: " << flag << endl;

                Serial << "x pos: " << x_pos << "[in]"<< endl;
                Serial << "y pos: " << y_pos << "[in]"<< endl;

                Serial << "x ref: " << x_ref << "[in]"<< endl;
                Serial << "y ref: " << y_ref << "[in]"<< endl;

                Serial << "x error: " << x_dist << "[in]"<< endl;
                Serial << "y error: " << y_dist << "[in]"<< endl << endl;
                
                Serial << "x vel: " << x_vel << "[in]"<< endl;
                Serial << "y vel: " << y_vel << "[in]"<< endl;

                Serial << "x vel ref: " << xdot_ref << "[in]"<< endl;
                Serial << "y vel ref: " << ydot_ref << "[in]"<< endl;

                Serial << "x vel error: " << xdot_ref - x_vel << "[in]"<< endl;
                Serial << "y vel error: " << ydot_ref - y_vel << "[in]"<< endl << endl;

                Serial << "xPWM: " << xCONT.PWM << "%" << endl;
                Serial << "yPWM: " << yCONT.PWM << "%" << endl 
                <<"-----------------------------------------------------------------------------------------------------" <<endl;
            }

        }
        else if(state ==4)
        {
            mag.disable();
        }
        vTaskDelay(delay_val);
    }
}