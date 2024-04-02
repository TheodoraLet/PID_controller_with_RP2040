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

typedef unsigned int uint;
//#define ENC_PIN1 26
//#define ENC_PIN2 7
#define pwm_pin 8
#define pin1 27
#define pin2 6

void delay(int iter){
int volatile counter;
counter=0;
while(counter<iter){
 ++counter;
}

}


int main(void) {
__enable_irq();
pinMode(pin1,1);
pinMode(pin2,1);
pwm_set_format(pwm_pin,255);

timer_init(3,2);

enc_pin(ENC_PIN1,0,ENC_PIN2);
target_angle=90;

while(1){
pid_calculations();

Motor_Drive();

}


}

/*************************** End of file ****************************/
