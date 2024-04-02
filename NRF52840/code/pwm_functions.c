#include "nrf52840.h"
#include "pwm_functions.h"
#include "nrf52840_bitfields.h"
#include "gpio_functions.h"
#include "stdio.h"

void pwm_set_format(uint pwm_pin,uint top){
NRF_CLOCK->TASKS_HFCLKSTART=1;
while(NRF_CLOCK->EVENTS_HFCLKSTARTED==0){};
int mask=(1UL<<16);
if(NRF_CLOCK->EVENTS_HFCLKSTARTED==1){printf("oscillator started\n");}
int clk_src=(NRF_CLOCK->HFCLKSTAT& 0b1);
printf("closk source %d\n",clk_src);
while((NRF_CLOCK->HFCLKSTAT &mask)==0){printf("waiting for clock to run\n");};
pinMode(pwm_pin,1);// set pin as output
pin_write(pwm_pin,0);// set pin low
NRF_PWM0->PSEL.OUT[0]=(pwm_pin<<PWM_PSEL_OUT_PIN_Pos)|(PWM_PSEL_OUT_CONNECT_Connected<<PWM_PSEL_OUT_CONNECT_Pos);
NRF_PWM0->ENABLE=(PWM_ENABLE_ENABLE_Enabled<<PWM_ENABLE_ENABLE_Pos);
NRF_PWM0->MODE=(PWM_MODE_UPDOWN_Up<<PWM_MODE_UPDOWN_Pos);// up mode
NRF_PWM0->PRESCALER=(PWM_PRESCALER_PRESCALER_DIV_16<<PWM_PRESCALER_PRESCALER_Pos);// div by 16-->1 MHz   // 0-16MHz,1-8MHz,2-4MHz,3-8MHz, 4-1MHz
NRF_PWM0->COUNTERTOP=top;// set maximum value to count for !!!!!! for values >2  , range[3~ 32767]
NRF_PWM0->LOOP=(PWM_LOOP_CNT_Disabled<<PWM_LOOP_CNT_Pos); // number of playbacks of pattern cycles
NRF_PWM0->DECODER=(PWM_DECODER_LOAD_Individual<<PWM_DECODER_LOAD_Pos)|(PWM_DECODER_MODE_RefreshCount<<PWM_DECODER_MODE_Pos); // LOAD Individual first 16 bits in all 4 PWM channels, or MODE refresh count
}

void pwm_write(uint pwm_pin,uint chan,uint16_t buf[1]){
NRF_PWM0->SEQ[chan].PTR=((uint32_t)(buf)<<PWM_SEQ_PTR_PTR_Pos) ;
NRF_PWM0->SEQ[chan].CNT=(sizeof(uint16_t)<<PWM_SEQ_CNT_CNT_Pos);//(sizeof(buf)/sizeof(uint16_t));
NRF_PWM0->SEQ[chan].REFRESH=0;
NRF_PWM0->SEQ[chan].ENDDELAY=0;
NRF_PWM0->TASKS_SEQSTART[chan]=1;
}