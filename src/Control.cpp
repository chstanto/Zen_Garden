/** @file Control.cpp
 *  This class is responsible for computing the PWM for motors based off of position PD Control
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

#include "Control.h"

Control::Control(void)
{

}

void Control::run(float pos_ref, float pos, float vel_ref, float vel)
{

    PWM =  Kp*(pos_ref-pos) + Kd*(vel_ref - vel);
    if(PWM>100)
    {
        PWM = 100;
    }
    else if(PWM<-100)
    {
        PWM = -100;
    }

}
