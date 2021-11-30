/** @file EMDriver.cpp
 *  This class is responsible for setting up a driver to interact with the electromagnet
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#include <EMDriver.h>

EMDriver::EMDriver(uint8_t ENP)
{
    EN = ENP;
    pinMode(EN, OUTPUT);     

}

void EMDriver::enable(void)
{
    digitalWrite(EN, HIGH);
}

void EMDriver::disable(void)
{
    digitalWrite(EN, LOW);
}