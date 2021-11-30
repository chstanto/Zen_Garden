/** @file ControlTask.cpp
 *  This file contains a task for controlling the Zen Gardener's magnet position
 *  @author  Aaron Tran
 *  @date    2021-Nov-29 Original file
 */

#include <Arduino.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "ControlTask.h"

void task_control(void* p_params)
{
    (void)p_params;                             // Shuts up a compiler warning
    vTaskDelay (20);
}
    