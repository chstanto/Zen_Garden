/** @file Control.cpp
 *  This class is responsible for computing the PWM for motors based off of position PD Control
 *  @author  Aaron Tran
 *  @author  Cole Stanton
 *  @date    2021-Nov-30 Original file
 */

#include "Control.h"

/** @brief  Instantiates control system object
 *  @param  vel_gain positional derivative gain, or velocity gain
 *  @param  motP2 positional proportional gain, or positional gain
 */
Control::Control(float vel_gain, float pos_gain)
{
    Kd = vel_gain;
    Kp = pos_gain;
}

/** @brief  Computes PWM based on positional error and velocity error
 *  @details This controller makes use of positional PD control. It also tries to correct for the deadzone observed in the system due to stiction. The PWM values saturate at +/- 100%.
 *  @param  pos_ref reference posiotional value
 *  @param  pos postional reading value
 *  @param  vel_ref reference velocity value
 *  @param  vel velocity reading value
 */
void Control::run(float pos_ref, float pos, float vel_ref, float vel)
{
    //Derivative control contribution 
    PWM_vel =  100*Kd*(vel_ref - vel);
    
    //Positional control contribution
    PWM_pos =  100*Kp*(pos_ref-pos);
    PWM =  PWM_vel + PWM_pos;

    //Correct for dead zone
    if (PWM >= 0)
    {
        PWM = PWM*87/100 + 13;
    }
    else
    {
        PWM = PWM*87/100 - 13;
    }

    //Saturate PWM values
    if (PWM > 100)
    {
        PWM = 100;
    }
    else if (PWM < -100)
    {
        PWM = -100;
    }

}
