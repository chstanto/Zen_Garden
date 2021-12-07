/** @file MOTyTask.cpp
 *  This file contains a task for controlling the motor in the y direction
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h"

#include "MOTyTask.h"
#include "MotorDriver.h"
#include "EncoderDriver.h"

//Motor 2 Pins
#define inputB1 PA8
#define inputB2 PA9
#define enableB PA5

//Encoder 2 Pins
#define E2CHA PB4
#define E2CHB PB5

void task_MOTy(void* p_params)
{
    
}