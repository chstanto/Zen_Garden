/** @file MotorDriver.h
 *  This class is responsible for setting up a driver to interact with motors
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

class MotorDriver
{
    protected:
    public:
    MotorDriver(uint8_t motP1, uint8_t motP2, uint8_t REAL_EN);
    void Enable_MOT(void);
    void Disable_MOT(void);
    void set_duty (float duty);

    uint8_t motorpin1;
    uint8_t motorpin2;
    uint8_t R_EN;
    float last_duty = 0;
};