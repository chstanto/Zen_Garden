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
    
    //MOTx handling
    /*
    self.CTRL1.Kp = shares.Kp
    ##@brief motor 1 measured speed [RPM]
    self.omega_meas1 = 60*self.enc1.get_delta()/0.020/360
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
    