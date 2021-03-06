/** @file DataTask.cpp
 *  This file contains a task for sending and receiving data to the Control Task
 * 
 *  Source code available here:
 *  https://github.com/chstanto/Zen_Garden/blob/main/src/DataTask.cpp
 *  @author  Aaron Tran
 *  @author  Cole Stanton
 *  @date    2021-Dec-1 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h" 
#include "DataTask.h"

//Square Design
// float ind = 1.25;
// float x_ref[21] = {2, 2  , 2  , 2  , 2  , 2  , 2+ind, 2+2*ind, 2+3*ind, 2+4*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+4*ind, 2+3*ind, 2+2*ind, 2+ind, 2};
// float y_ref[21] = {2, 2+ind, 2+2*ind, 2+3*ind, 2+4*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+5*ind, 2+4*ind, 2+3*ind, 2+2*ind, 2+ind, 2 , 2  , 2  , 2  , 2  , 2};
// uint8_t data[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
// uint8_t length = 21;

//Circle Design
/// @brief preset reference x position data
float x_ref[37] = {7.000,6.954,6.819,6.598,6.298,5.928,5.500,5.026,4.521,4.000,3.479,2.974,2.500,2.072,1.702,1.402,1.181,1.046,1.000,1.046,1.181,1.402,1.702,2.072,2.500,2.974,3.479,4.000,4.521,5.026,5.500,5.928,6.298,6.598,6.819,6.954,7.000};
/// @brief preset reference y position data
float y_ref[37] = {4.000,4.521,5.026,5.500,5.928,6.298,6.598,6.819,6.954,7.000,6.954,6.819,6.598,6.298,5.928,5.500,5.026,4.521,4.000,3.479,2.974,2.500,2.072,1.702,1.402,1.181,1.046,1.000,1.046,1.181,1.402,1.702,2.072,2.500,2.974,3.479,4.000};
/// @brief preset data availability array
uint8_t data[37] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
/// @brief length of data arrays
uint8_t length = 37;

/** @brief   Task which sends data to ControlTask.h
 *  @details This task sends designs Zen Garden designs to the Control Task. Upon further development, this task would also read from a CSV to obtain that data, retrieve performance data, and print desired data to a CSV.
 *  @param   p_params A pointer to function parameters which we don't use.
 */
void task_data(void* p_params)
{
    uint8_t index = 0;
    for(;;)
    {
        //Place design data in queue to send to Control Task
        xref.put(x_ref[index]);
        yref.put(y_ref[index]);
        data_NOTavail.put(data[index]);
        if(index<length)
        {
        index++;
        }
    }
    vTaskDelay(20);
}