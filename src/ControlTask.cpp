/** @file ControlTask.cpp
 *  This file contains a task for controlling the Zen Gardener's magnet position
 *  @author  Aaron Tran
 *  @date    2021-Nov-29 Original file
 */


#include <Arduino.h>
#include <PrintStream.h>
#if (defined STM32L4xx || defined STM32F4xx)
    #include <STM32FreeRTOS.h>
#endif
#include "shares.h" 

#include "ControlTask.h"
#include "EncoderDriver.h"

//Encoder Pins
#define E1CHA PA0
#define E1CHB PA1

#define E2CHA PB4
#define E2CHB PB5

void task_control(void* p_params)
{
    (void)p_params;                             // Shuts up a compiler warning
    
    uint8_t state = 0;
    for(;;)
    {
        if(state ==0) //INIT State, find home setup everything
        {
            STM32Encoder enc1 (TIM5, E1CHA, E1CHB);
            STM32Encoder enc2 (TIM3, E2CHA, E2CHB);
            state = 1;
        }
        else if(state ==1)
        {
            state = 2;
        }
        vTaskDelay(20);
    }




    //MOTx handling
    /*
    ##@brief motor 1 measured speed [RPM]
    x_vel = 
    self.CTRL1.run(self.omega_ref[self.n],self.omega_meas1, self.theta_ref[self.n], self.enc1.get_position())   
    self.mot1.set_duty(self.CTRL1.PWM)
    self.enc1.run()
    shares.enc1_err = (self.omega_ref[self.n] - self.omega_meas1)**2 +  (self.theta_ref[self.n] - self.enc1.get_position())**2           
    shares.enc1_pos = self.enc1.get_position()
    shares.enc1_speed = self.omega_meas1     
    */
    //MOTy handling

    
    
    vTaskDelay (20);
}
    