#include "nrf52840.h"
#include "gpio_functions.h"
#include "pid_functions.h"
#include "stdio.h"
//#define ENC_PIN_2 6 


void GPIOTE_IRQHandler(void){
int b=(NRF_P0->IN >> ENC_PIN2)& 1UL;
if(b==1){
pos++;
}else{
pos--;
}
NRF_GPIOTE->EVENTS_IN[0]=0;
}