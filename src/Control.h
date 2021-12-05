/** @file Control.h
 *  This class is responsible for computing the PWM for motors based off of position PD Control
 *  @author  Aaron Tran
 *  @author  Cole Stanton
 *  @date    2021-Nov-30 Original file
 */

/** @brief   Class for implementing Mechatronic Zen Garden control system
*/
class Control
{
    protected:
    public:

    //Constructor
    Control(float vel_gain, float pos_gain);
    
    //Runs control system to calculate PWM
    void run(float pos_ref, float pos, float vel_ref, float vel);
    
    float Kd = 1.003; // Derivative gain
    float Kp = 0.0203; // Proportional gain

    //PWM values
    float PWM_vel;
    float PWM_pos;
    float PWM;
};