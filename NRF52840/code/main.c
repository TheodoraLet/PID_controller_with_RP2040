#include <stdio.h>
#include <stdlib.h>
#include <nrf52840.h>
#include "pwm_functions.h"
#include "gpio_functions.h"
#include "math.h"
#include "pid_functions.h"
#include "nrf52840_bitfields.h"
#include "uart_functions.h"
#include "encoder_function.h"
#include "timer_functions.h"
#include "spi_functions.h"
#include "encoder_function.h"
#include "app_uart.h"
#include "app_error.h"
#include "nrf_uart.h"
#include "nrf_delay.h"

#define pwm_pin 8
#define pin1 27
#define pin2 6


int main(void)
{
   __enable_irq();
   // initialize uart driver
   uart_init();

    
    int port=0;
    // initialize fatfs 
    fatfs_initialization();
    // set driver pins as output
    pinMode(pin1,1);
    pinMode(pin2,1);
    // initialize pwm duty cycle
    pwm_set_format(pwm_pin,255);

    // set direction of motor (optional)
    pin_write(pin1,1);
    pin_write(pin2,0);
    //initiliaze timer 
    timer_init(3,2);
    // initialize encoder pins and GPIOTE event
    enc_pin(ENC_PIN1,port,ENC_PIN2);

     while(1){
     
     // calculate position and error
      pid_calculations();
 
     // update the duty cycle to reach target 
      Motor_Drive();

     // write data to file 
      fatfs_write();
     }
}

/** @} */
