#include "nrf52840.h"
#include "nrf52840_bitfields.h"

void enc_pin(int pin_number1,int port,int pin_number2){ 
  //select config chaneel from 0 to 7
  NRF_GPIOTE->CONFIG[0]=0;
  //configure encoder pin 1 as input pin to create event when from low to high 
  NRF_GPIOTE->CONFIG[0]=(pin_number1<<GPIOTE_CONFIG_PSEL_Pos)|(GPIOTE_CONFIG_MODE_Event<<GPIOTE_CONFIG_MODE_Pos)|
                                   (port<<GPIOTE_CONFIG_PORT_Pos)|(GPIOTE_CONFIG_POLARITY_LoToHi<<GPIOTE_CONFIG_POLARITY_Pos);
  
  // connect input buffer for encoder pin 1
  NRF_P0->PIN_CNF[pin_number1]|=(GPIO_PIN_CNF_INPUT_Connect<<GPIO_PIN_CNF_INPUT_Pos);
  // set encoder pin 2 as input and connect the input buffer 
  NRF_P0->PIN_CNF[pin_number2]=(GPIO_PIN_CNF_DIR_Input<<GPIO_PIN_CNF_DIR_Pos)|(GPIO_PIN_CNF_INPUT_Connect<<GPIO_PIN_CNF_INPUT_Pos);
  // enable interrupt for IN event for config channel 0 (low to high)
  NRF_GPIOTE->INTENSET|=(GPIOTE_INTENSET_IN0_Set<<GPIOTE_INTENSET_IN0_Pos);
  // clear pending interrupts
  NVIC_ClearPendingIRQ(GPIOTE_IRQn);
  // enable GPIOTE interrupt
  NVIC_EnableIRQ(GPIOTE_IRQn);
}