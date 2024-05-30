#include "nrf52840.h"
#include "pwm_functions.h"
#include "nrf52840_bitfields.h"
#include "gpio_functions.h"
#include "stdio.h"

void pwm_set_format(uint pwm_pin,uint top){
  // start peripheral clock
  NRF_CLOCK->TASKS_HFCLKSTART=1; 
   // wait for clock to start
  while(NRF_CLOCK->EVENTS_HFCLKSTARTED==0){};
  int mask=(1UL<<16);
  //debugging messages
  if(NRF_CLOCK->EVENTS_HFCLKSTARTED==1){ 
    printf("oscillator started\n");
  }
  // print the clock source for debugging
  int clk_src=(NRF_CLOCK->HFCLKSTAT& 0b1); 
  printf("closk source %d\n",clk_src);
  // in case clock has not started
  while((NRF_CLOCK->HFCLKSTAT &mask)==0){
    printf("waiting for clock to run\n"); 
  };
  // set pin as output
  pinMode(pwm_pin,1);
  // set pin low (optional)
  pin_write(pwm_pin,0);
  //configure and select the pwm pin 
  NRF_PWM0->PSEL.OUT[0]=(pwm_pin<<PWM_PSEL_OUT_PIN_Pos)|(PWM_PSEL_OUT_CONNECT_Connected<<PWM_PSEL_OUT_CONNECT_Pos);
  //enable the pwm peripheral 0 instance
  NRF_PWM0->ENABLE=(PWM_ENABLE_ENABLE_Enabled<<PWM_ENABLE_ENABLE_Pos);
  //up mode
  NRF_PWM0->MODE=(PWM_MODE_UPDOWN_Up<<PWM_MODE_UPDOWN_Pos);
  // div by 16-->1 MHz   // 0-16MHz,1-8MHz,2-4MHz,3-8MHz, 4-1MHz
  NRF_PWM0->PRESCALER=(PWM_PRESCALER_PRESCALER_DIV_16<<PWM_PRESCALER_PRESCALER_Pos);
  // set maximum value to count for !!!!!! for values >2  , range[3~ 32767]
  NRF_PWM0->COUNTERTOP=top;
  // number of playbacks of pattern cycles
  NRF_PWM0->LOOP=(PWM_LOOP_CNT_Disabled<<PWM_LOOP_CNT_Pos);
  // LOAD Individual first 16 bits in all 4 PWM channels, or MODE refresh count
  NRF_PWM0->DECODER=(PWM_DECODER_LOAD_Individual<<PWM_DECODER_LOAD_Pos)|(PWM_DECODER_MODE_RefreshCount<<PWM_DECODER_MODE_Pos); 
}

void pwm_write(uint pwm_pin,uint chan,uint16_t buf[1]){
 //set the buffer to store the duty cycle
  NRF_PWM0->SEQ[chan].PTR=((uint32_t)(buf)<<PWM_SEQ_PTR_PTR_Pos) ;
 //set the size of buffer
  NRF_PWM0->SEQ[chan].CNT=(sizeof(uint16_t)<<PWM_SEQ_CNT_CNT_Pos);
 // no additional pwm periods between samples
  NRF_PWM0->SEQ[chan].REFRESH=0;
  // no time added after the sequence
  NRF_PWM0->SEQ[chan].ENDDELAY=0;
  // start sequence
  NRF_PWM0->TASKS_SEQSTART[chan]=1;
}