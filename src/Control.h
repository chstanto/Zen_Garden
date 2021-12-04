/** @file Control.h
 *  This class is responsible for computing the PWM for motors based off of position PD Control
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

class Control
{
    protected:
    public:
    Control(float vel_gain, float pos_gain);
    void run(float pos_ref, float pos, float vel_ref, float vel);
    float Kd = 1.003; // Derivative gain
    float Kp = 0.0203; // Proportional gain

    float pos_ref; //Placeholder reference position, should be tak
    float vel_ref;

    float PWM_vel;
    float PWM_pos;
    float PWM;
};