/** @file MotorDriver.h
 *  This class is responsible for setting up a driver to interact with motors
 *  @author  Aaron Tran
 *  @author  Cole Stanton
 *  @date    28-Oct-2020 Original file
 */

/** @brief   Class for setting up BLDC motor interface
 *  @details MotorDriver class was developed for use with a Qunqi L298N motor driver chip
*/
class MotorDriver
{
    protected:
    public:
    //Constructor
    MotorDriver(uint8_t motP1, uint8_t motP2, uint8_t REAL_EN);
    
    //Functions to send PWM signals to motor
    void Enable_MOT(void);
    void Disable_MOT(void);
    void set_duty (float duty);

    //Pins
    uint8_t motorpin1;
    uint8_t motorpin2;
    uint8_t R_EN;
    
    //Most recent duty 
    float last_duty = 0;
};