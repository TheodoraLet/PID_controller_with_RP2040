#include "nrf52840.h"
#include "gpio_functions.h"
#include "motor_variable.h"

void TIMER0_IRQHandler(void){
motor1^=1;
pin_write(7,motor1);
}