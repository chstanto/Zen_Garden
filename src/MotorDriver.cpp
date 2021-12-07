/** @file MotorDriver.cpp
 *  This class is responsible for setting up a driver to interact with motors
 * 
 *  Source code available here:
 *  @c https://github.com/chstanto/Zen_Garden/blob/main/src/MotorDriver.cpp
 *  @author  Aaron Tran
 *  @author  Cole Stanton
 *  @date    28-Oct-2020 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#include <MotorDriver.h>


/** @brief  Instantiates motor driver object for certain pin connections
 *  @param  motP1 Motor input pin 1
 *  @param  motP2 Motor input pin 2
 *  @param  REAL_EN Motor enable pin
 */
MotorDriver::MotorDriver(uint8_t motP1, uint8_t motP2, uint8_t REAL_EN)
{
  motorpin1 = motP1;
  motorpin2 = motP2;
  R_EN = REAL_EN;

  // Configure all pins as ouput pins
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT); 
  pinMode(REAL_EN, OUTPUT);

}

/** @brief   Turns motor on
 *  @details Enables motor with the same PWM prior to disabling the motor
 */
void MotorDriver::Enable_MOT(void)
{
  analogWrite(R_EN, last_duty);
}

/** @brief   Turns motor off
 *  @details Sends motor a 0% PWM signal
 */
void MotorDriver::Disable_MOT(void)
{
  analogWrite(R_EN, 0);
}

/** @brief   Sends motor PWM signals
 *  @details Takes PWM signal on 0-100% scale and converts it to the 0-255 scale accepted by the enable pin. Positive versus negative values determine direction of spin
 *  @param   duty Desired PWM value. Values between -100 and 100 are valid.
 */
void MotorDriver::set_duty (float duty)
{
  last_duty = duty*255/100;
  if (duty > 0)
  {
    digitalWrite(motorpin1, HIGH);
    digitalWrite(motorpin2, LOW);
  }
  else
  {
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, HIGH); 
    duty *= -1;

  };
  analogWrite(R_EN, duty*255/100);
  
}
