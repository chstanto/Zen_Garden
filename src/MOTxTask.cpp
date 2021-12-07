/** @file MOTxTask.cpp
 *  This file contains a task for controlling the motor in the x direction
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h"

#include "MOTxTask.h"
#include "MotorDriver.h"
#include "EncoderDriver.h"

//Motor 1 Pins
#define inputA1 PB8
#define inputA2 PB9
#define enableA PA6

//Encoder 1 Pins
#define E1CHA PA0
#define E1CHB PA1

void task_MOTx(void* p_params)
{
    
}