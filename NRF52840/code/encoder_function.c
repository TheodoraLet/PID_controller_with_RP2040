#include "nrf52840.h"
#include "nrf52840_bitfields.h"

void enc_pin(int pin_number1,int port,int pin_number2){ // config_channel from 0 to 7
NRF_GPIOTE->CONFIG[0]=0;
NRF_GPIOTE->CONFIG[0]=(pin_number1<<GPIOTE_CONFIG_PSEL_Pos)|(GPIOTE_CONFIG_MODE_Event<<GPIOTE_CONFIG_MODE_Pos)|
                                 (port<<GPIOTE_CONFIG_PORT_Pos)|(GPIOTE_CONFIG_POLARITY_LoToHi<<GPIOTE_CONFIG_POLARITY_Pos);
NRF_P0->PIN_CNF[pin_number1]|=(GPIO_PIN_CNF_INPUT_Connect<<GPIO_PIN_CNF_INPUT_Pos);// enable pull down resistor for input number
NRF_P0->PIN_CNF[pin_number2]=(GPIO_PIN_CNF_DIR_Input<<GPIO_PIN_CNF_DIR_Pos)|(GPIO_PIN_CNF_INPUT_Connect<<GPIO_PIN_CNF_INPUT_Pos);
NRF_GPIOTE->INTENSET|=(GPIOTE_INTENSET_IN0_Set<<GPIOTE_INTENSET_IN0_Pos);// enable interrupt for IN event for config channel 0
NVIC_ClearPendingIRQ(GPIOTE_IRQn);
NVIC_EnableIRQ(GPIOTE_IRQn);
}