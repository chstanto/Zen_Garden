/** @file DataTask.cpp
 *    
 * 
 *  @author Cole Stanton
 *  @date   2021-Nov-30 Original file
 */

#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "DataTask.h"

#include <fstream>
#include <iostream>

/** @brief   
 *  @details 
 *  @param   
 */

/*
void task_show_data (void* p_params)
{
    uint8_t state = 0;                 // State machine takes data or doesn't
    uint16_t counter;                  // Counts number of data points recorded

    for(;;)
    {
        // In state 0 we're waiting for signal from control task to pull data 
        if (state == 0)
        {
            std::ofstream Input_Data;
            Input_Data.open("square_data.csv");
            int x , y ;

            while (getline(Input_Data, x, ',')) 
            {
                x_queue.put(x);

                getline(Input_Data, y) ;
                y_queue.put(y);
            }
            Input_Data.close;
            state = 1;
        }
        // In state 1 data is being recorded until the queue is empty
        else if (state == 1)
        {
            std::ofstream Output_Data;
            Output_Data.open("Output_Data.csv");
            Output_Data << "X Motor,X Encoder,Y Motor,Y Encoder \n";

            // Check if we've taken the required number of data points yet
            if (encoder_queue.get())
            {
                Output_Data << encoder_queue.get() << motor_queue.get() << "\n";
                
            }
        }

        // This task always runs once every 5 ms
        vTaskDelay (5);
    }
}

*/
