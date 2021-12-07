/** @file EMTask.cpp
 *  This file contains a task for controlling the electromagnet
 *  @author  Aaron Tran
 *  @date    2021-Nov-30 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h"

#include "EMTask.h"
#include "EMDriver.h"

//Electromagnet Pin
#define MagPin PA10
void task_EM(void* p_params)
{

}