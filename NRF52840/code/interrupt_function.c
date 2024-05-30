#include "nrf52840.h"
#include "gpio_functions.h"
#include "pid_functions.h"
#include "stdio.h"
 


void GPIOTE_IRQHandler(void){
  //read the state of the encoder pin 
  int b=(NRF_P0->IN >> ENC_PIN2)& 1UL;
  //if pin is high increase for clockwise rotation
  if(b==1){
  pos++;
  //if pin is low decrease for anticlockwise rotation
  }else{
  pos--;
  }
  //clear the event so the interrupt will happen again 
  NRF_GPIOTE->EVENTS_IN[0]=0;
 }
