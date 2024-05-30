#include "nrf52840.h"
#include "timer_functions.h"
#include "stdio.h"

void timer_init(int bit_width,int presc){
  // start oscillator 
 NRF_CLOCK->TASKS_HFCLKSTART=1;
  // check if oscillator started 
  while(NRF_CLOCK->EVENTS_HFCLKSTARTED==0){};
  int mask=(1UL<<16);
  if(NRF_CLOCK->EVENTS_HFCLKSTARTED==1){
    printf("oscillator started\n");
  }
  int clk_src=(NRF_CLOCK->HFCLKSTAT& 0b1);
  printf("closk source %d\n",clk_src);
  while((NRF_CLOCK->HFCLKSTAT &mask)==0){
    printf("waiting for clock to run\n");
  };

  // 0-16bit,1-8bit,2-24bit,3-32bi
  NRF_TIMER0->BITMODE=bit_width;
  // set the power of 2 e.g 16/(2^2) 4 MHz
  NRF_TIMER0->PRESCALER=2; 
  NRF_TIMER0->TASKS_START=1;
}

int time_interval(int chan){ 
  // trigger capture timer register
  NRF_TIMER0->TASKS_CAPTURE[chan]=1; 
  //*1e-03;//*2.5*1e-07; // return time with bitwidth as chosen
  int res=NRF_TIMER0->CC[chan];
  // clear timer so it will return the deltaT between two sequential calls
  NRF_TIMER0->TASKS_CLEAR=1; 
  return res;
}