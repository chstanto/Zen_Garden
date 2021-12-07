/** @file Control.cpp
 *  This class is responsible for computing the PWM for motors based off of position PD Control
 *  
 *  Source code available here:
 *  @c https://github.com/chstanto/Zen_Garden/blob/main/src/Control.h
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
    
    /// @brief Derivative gain
    float Kd = 1.003; 

    /// @brief Proportional gain
    float Kp = 0.0203; 

    /// @brief velocity error contribution to PWM
    float PWM_vel;

    /// @brief position error contribution to PWM
    float PWM_pos;

    /// @brief PWM sent to motors
    float PWM;
};