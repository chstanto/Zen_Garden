/** @file ControlTask.cpp
 *  This file contains a task for controlling the Zen Gardener's magnet position
 * 
 *  Source code available here:
 *  https://github.com/chstanto/Zen_Garden/blob/main/src/ControlTask.cpp
 *  @author  Aaron Tran
 *  @author  Cole Stanton
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




/** @brief   Task which sends data to ControlTask.h
 *  @details This task sends designs Zen Garden designs to the Control Task. Upon further development, this task would also read from a CSV to obtain that data, retrieve performance data, and print desired data to a CSV.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_control(void* p_params)
{
    Serial.begin (115200);
    delay(5000);
    //Setup Code
    (void)p_params;                             // Shuts up a compiler warning
    uint8_t state = 0;
    //Position and velocity declarations

    /// @brief Current x position, [in]       
    float x_pos = 0;      
    /// @brief Current y position, [in]     
    float y_pos = 0;       
    /// @brief Current x velocity, [in/s]
    float x_vel = 0;
    /// @brief Current y velocity, [in/s]
    float y_vel = 0;    
    /// @brief Previous x position reading, [in]    
    float x_last = 0;  
    /// @brief Previous y position reading, [in]    
    float y_last = 0;    
    /// @brief Reference x position, [in]   
    float x_ref = 0;        
    /// @brief Reference y position, [in]
    float y_ref = 0;    
    /// @brief Reference x velocity, [in/s]    
    float xdot_ref = 0;    
    /// @brief Reference y velocity, [in/s] 
    float ydot_ref = 0;    
    /// @brief Desired operating speed, [in/s] 
    float speed = 1;  
    /// @brief x error, [in]      
    float x_dist = 0;       
    /// @brief y error, [in]
    float y_dist = 0;      
    /// @brief Angle of reference velocity vector [rad]
    float ang = 0;        
    /// @brief Control system's acceptable error [in]  
    float err = 0.25;        
    /// @brief Clock value [ms]
    unsigned long last_time = 0;  
    /// @brief vTaskDelay value [ms]  
    uint8_t delay_val = 10; 
    
    //Configure limit switch pins as input pullups
    pinMode(limx, INPUT_PULLUP); 
    pinMode(limy, INPUT_PULLUP);

    //Flag values
    /// @brief triggers flag if x limit switch has been clicked
    uint8_t xNOThome;     
    /// @brief triggers flag if y limit switch has been clicked  
    uint8_t yNOThome;     
    /// @brief triggers flag if first x position has been reached  
    bool firstx = false;    
    /// @brief triggers flag if first y position has been reached
    bool firsty = false;   
    /// @brief triggers flag if x position error is sufficiently low
    bool x_ok = false;    
    /// @brief triggers flag if y position error is sufficiently low 
    bool y_ok = false;    
    /// @brief triggers flag if last reference value has been received 
    uint8_t flag;          

    //Driver setups
    MotorDriver yMOT (inputA1, inputA2, enableA);
    MotorDriver xMOT (inputB1, inputB2, enableB);
    STM32Encoder yENC (TIM2, E1CHA, E1CHB);
    STM32Encoder xENC (TIM3, E2CHA, E2CHB);
    Control xCONT (0.01, 0.38);
    Control yCONT (0.012, 0.5);
    EMDriver mag (MagPin);

    
    for(;;)
    {
        if(state ==0) //INIT State, find home setup everything
        {
            mag.enable();
            xNOThome = digitalRead(limx);
            yNOThome = digitalRead(limy);
            //Turn on x motor until it reaches zero position
            if (xNOThome)
            {
                xMOT.set_duty(-25); 
            }
            else
            {
                xENC.zero();
                xMOT.Disable_MOT();
            }
            //Turn on y motor until it reaches zero position
            if (yNOThome)
            {
                yMOT.set_duty(-25);
            }
            else
            {
                yENC.zero();
                yMOT.Disable_MOT();
            }
            //Once both motors have reached the home position, transition to state 1
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
                flag = data_NOTavail.get();
                state = 1;
            }
        }
        else if(state ==1) //Preparation State, move motor to first position
        {
            //Read encoder values
            x_pos = -xENC.update()*1.571/4000;
            y_pos = -yENC.update()*1.571/4000;           
            
            //Trigger flags and turns off motor upon reaching first x position
            if (x_pos >= x_ref)
            {
                firstx = true;
                xMOT.Disable_MOT();
            }
            //Trigger flags and turns off motor upon reaching first y position
            if (y_pos >= y_ref)
            {
                firsty = true;
                yMOT.Disable_MOT();
            }
            //If both flags are triggered, transition to state 2
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
        else if(state ==2) //Drawing state
        {   
            //Read current values
            x_pos = -xENC.update()*1.571/4000;
            y_pos = -yENC.update()*1.571/4000;
            x_vel = (x_pos - x_last)/(millis()-last_time)*1000;
            y_vel = (y_pos - y_last)/(millis()-last_time)*1000;
            x_last = x_pos;
            y_last = y_pos;
            last_time = millis();

            //Determine reference velocities based off of positional error and desired speed
            x_dist = x_ref - x_pos;
            y_dist = y_ref - y_pos;
            ang = atan2(y_dist, x_dist);

            xdot_ref = speed*cos(ang);
            ydot_ref = speed*sin(ang);
            
            //Trigger flags when acceptable error is met and enter damping phase, where reference velocities are set to zero and velocity gain is reduced to damp out oscillations
            if( abs(x_dist)< err)
            {
                x_ok = true;
                xdot_ref = 0;
                xCONT.Kd = 0.00625;
            }
            else
            {
                xCONT.Kd = 0.012;
            }
            if( abs(y_dist)< err)
            {
                y_ok = true;
                ydot_ref = 0;
                yCONT.Kd = 0.00625;
            }
            else    
            {
                yCONT.Kd = 0.012; 
            }
            

            if (x_ok & y_ok & !flag)
            {
                x_ref = xref.get();
                y_ref = yref.get();
                flag = data_NOTavail.get();
                x_ok = false;
                y_ok = false;
                //Determine reference velocities
                x_dist = x_ref - x_pos;
                y_dist = y_ref - y_pos;
                ang = atan2(y_dist, x_dist);
                xdot_ref = speed*cos(ang);
                ydot_ref = speed*sin(ang);
            }
            
            //Check if last data point has been retrieved and move to state 3 if it has
            if (flag & x_ok & y_ok)
            {
                xMOT.Disable_MOT();
                yMOT.Disable_MOT();
                mag.disable();
                state = 3;
            }
            else
            {
                xCONT.run(x_ref, x_pos, xdot_ref, x_vel);
                xMOT.set_duty(xCONT.PWM);
                yCONT.run(y_ref, y_pos, ydot_ref, y_vel);
                yMOT.set_duty(yCONT.PWM);   
            }

            // Serial << "You are in state " << state << endl;
            // Serial << "Data not available?: " << flag << endl;
            // Serial << "x pos: " << x_pos << "[in]"<< endl;
            // Serial << "y pos: " << y_pos << "[in]"<< endl;
            // Serial << "x ref: " << x_ref << "[in]"<< endl;
            // Serial << "y ref: " << y_ref << "[in]"<< endl;
            // Serial << "x error: " << x_dist << "[in]"<< endl;
            // Serial << "y error: " << y_dist << "[in]"<< endl << endl;
            // Serial << "x vel: " << x_vel << "[in/s]"<< endl;
            // Serial << "y vel: " << y_vel << "[in/s]"<< endl;
            // Serial << "x vel ref: " << xdot_ref << "[in/s]"<< endl;
            // Serial << "y vel ref: " << ydot_ref << "[in/s]"<< endl;
            // Serial << "x vel error: " << xdot_ref - x_vel << "[in/s]"<< endl;
            // Serial << "y vel error: " << ydot_ref - y_vel << "[in/s]"<< endl << endl;
            // Serial << "xPWM_pos: " << xCONT.PWM_pos << "%" << endl;
            // Serial << "yPWM_pos: " << yCONT.PWM_pos << "%" << endl;            
            // Serial << "xPWM_vel: " << xCONT.PWM_vel << "%" << endl;
            // Serial << "yPWM_vel: " << yCONT.PWM_vel << "%" << endl;            
            // Serial << "xPWM: " << xCONT.PWM << "%" << endl;
            // Serial << "yPWM: " << yCONT.PWM << "%" << endl;
            // Serial <<"-----------------------------------------------------------------------------------------------------" <<endl;
        }
        else if(state ==3) //Idle
        {
            Serial << "State 3" << endl;
        }
        vTaskDelay(delay_val);
    }
}
