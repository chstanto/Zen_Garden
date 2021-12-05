/** @file EMDriver.cpp
 *  This class is responsible for setting up a driver to interact with the electromagnet
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#include <EMDriver.h>


/** @brief  Instantiates EMDriver object for enable pin
 *  @param  ENP enable pin, connected to gate of transistor
 */
EMDriver::EMDriver(uint8_t ENP)
{
    EN = ENP;
    pinMode(EN, OUTPUT);     

}

/** @brief  Turns on electromagnet al the way
 */
void EMDriver::enable(void)
{
    digitalWrite(EN, HIGH);
}

/** @brief  Turns off electromagnet
 */
void EMDriver::disable(void)
{
    digitalWrite(EN, LOW);
}