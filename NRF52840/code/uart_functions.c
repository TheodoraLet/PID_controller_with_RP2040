#include "nrf52840.h"
#include <nrf.h>
#include "gpio_functions.h"
#include "stdio.h"

void uart_config(int rxd,int txd){
pinMode(rxd,0);
pinMode(txd,1);
NRF_UARTE0->PSEL.RXD=(rxd<<UARTE_PSEL_RXD_PIN_Pos)|(UARTE_PSEL_RXD_CONNECT_Connected<<UARTE_PSEL_RXD_CONNECT_Pos);
NRF_UARTE0->PSEL.TXD=(txd<<UARTE_PSEL_TXD_PIN_Pos)|(UARTE_PSEL_TXD_CONNECT_Connected<<UARTE_PSEL_TXD_CONNECT_Pos);
NRF_CLOCK->TASKS_HFCLKSTART=1;
while(NRF_CLOCK->EVENTS_HFCLKSTARTED==0){};
int mask=(1UL<<16);
if(NRF_CLOCK->EVENTS_HFCLKSTARTED==1){printf("oscillator started\n");}
int clk_src=(NRF_CLOCK->HFCLKSTAT& 0b1);
printf("closk source %d\n",clk_src);
while((NRF_CLOCK->HFCLKSTAT &mask)==0){printf("waiting for clock to run\n");};
NRF_UARTE0->CONFIG|=(UARTE_CONFIG_HWFC_Disabled<<UARTE_CONFIG_HWFC_Pos)|(UARTE_CONFIG_PARITY_Excluded<<UARTE_CONFIG_PARITY_Pos)|
                        (UARTE_CONFIG_STOP_One<<UARTE_CONFIG_STOP_Pos); // disable flow control
NRF_UARTE0->BAUDRATE=(UARTE_BAUDRATE_BAUDRATE_Baud115200<<UARTE_BAUDRATE_BAUDRATE_Pos);//115200 baudrate
NRF_UARTE0->INTENSET=(UARTE_INTENSET_ERROR_Set<<UARTE_INTENSET_ERROR_Pos);
NRF_UARTE0->ENABLE=(UARTE_ENABLE_ENABLE_Enabled<<UARTE_ENABLE_ENABLE_Pos); //enable UARTE
}

void uart_write(uint8_t write_buffer[],uint8_t buffer_size){
NRF_UARTE0->TXD.MAXCNT=(buffer_size);//sizeof(write_buffer);
NRF_UARTE0->TXD.PTR=((uint32_t)(write_buffer)<<UARTE_TXD_PTR_PTR_Pos);

NRF_UARTE0->TASKS_STARTTX=1;

while(NRF_UARTE0->EVENTS_ENDTX==0){ //if last byte is transmitted equals 1

};

NRF_UARTE0->TASKS_STOPTX=1; // stop transmission
while(NRF_UARTE0->EVENTS_TXSTOPPED==0){ // when transmission stops equals 1

};

NRF_UARTE0->ENABLE=(UARTE_ENABLE_ENABLE_Disabled<<UARTE_ENABLE_ENABLE_Pos);
}


