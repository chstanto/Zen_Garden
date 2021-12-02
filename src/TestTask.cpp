/** @file TestTask.cpp
 *  This file contains a task for adding data for tests
 *  @author  Aaron Tran
 *  @date    2021-Dec-1 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h" 

//Setup code
float x_ref[21] = {2, 2  , 2  , 2  , 2  , 2  , 2.1, 2.2, 2.3, 2.4, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.4, 2.3, 2.2, 2.1, 2};
float y_ref[21] = {2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.4, 2.3, 2.2, 2.1, 2  , 2  , 2  , 2  , 2  , 2};
uint8_t data[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1};
void task_test(void* p_params)
{
/*
    for(uint8_t index; index<5; index++)
    {
        xref.put(x_ref[index]);
        yref.put(y_ref[index]);
        data_NOTavail.put(data[index]);
    }
*/
    uint8_t index = 0;
    for(;;)
    {
        xref.put(x_ref[index]);
        yref.put(y_ref[index]);
        data_NOTavail.put(data[index]);
        if(index<21)
        {
        index++;
        }
    }
    vTaskDelay(20);
}