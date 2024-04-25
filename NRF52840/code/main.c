/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>
#include <nrf52840.h>
#include "pwm_functions.h"
#include "gpio_functions.h"
#include "math.h"
#include "pid_functions.h"
#include "nrf52840_bitfields.h"
#include "uart_functions.h"
#include "interrupt_function.h"
#include "encoder_function.h"
#include "timer_functions.h"
/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/

#define pwm_pin 8
#define pin1 27
#define pin2 6
#define rxd 11
#define txd 12


uint32_t read_buffer[4];
extern volatile uint8_t read_size=0;


int main(void) {
__enable_irq();
pinMode(pin1,1);
pinMode(pin2,1);
pwm_set_format(pwm_pin,255);
uart_config(rxd,txd);
pin_write(pin1,1);
pin_write(pin2,0);
timer_init(3,2);

enc_pin(ENC_PIN1,0,ENC_PIN2);



while(1){
if(pass){
uart_read(read_buffer);
char writebuff[read_size+2];
for (int i=0;i<=read_size;i++) sprintf(&writebuff[i],"%d",read_buffer[i]-'0');

uart_write(writebuff);
target_angle=atoi(writebuff);
pass=0;
}

pid_calculations();

Motor_Drive();


}


}

/*************************** End of file ****************************/
