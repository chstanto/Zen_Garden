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
//#include "square_data.csv"
//#include "fstream.h"

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
            if (pull_data.get ())
            {

                fstream input;
                input.open("square_data.csv", ios::in);

                // Read the Data from the file
                // as String Vector
                vector<string> row;
                string line, word, temp;
            
                while (input >> temp) 
                {
            
                    row.clear();
            
                    // read an entire row and
                    // store it in a string variable 'line'
                    getline(input, line);
            
                    // used for breaking words
                    stringstream s(line);
            
                    // read every column data of a row and
                    // store it in a string variable, 'word'
                    while (getline(s, word, ', ')) {
            
                        // add all the column data
                        // of a row to a vector
                        row.push_back(word);
                    }


                while()
                pull_data.put (false);      // So we don't record forever
                counter = 0;
                state = 1;
            }
        }

        // In state 1 data is being recorded until the queue is empty
        else if (state == 1)
        {
            // Read one data point and immediately stuff it into the queue
            data_queue.put (analogRead (DATA_ACQ_PIN));

            // Check if we've taken the required number of data points yet
            if (++counter > POINTS_PER_SET)
            {
                state = 0;
            }
        }

        // This task always runs once every 5 ms
        vTaskDelay (5);
    }
}
*/