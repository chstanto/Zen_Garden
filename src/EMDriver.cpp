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