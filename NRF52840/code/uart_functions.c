#include "nrf52840.h"
#include <nrf.h>
#include "gpio_functions.h"
#include "stdio.h"
#include "string.h"

extern volatile uint8_t read_size;

void uart_config(int rxd,int txd){
  NRF_P0->PIN_CNF[rxd]|=(GPIO_PIN_CNF_DIR_Input<<GPIO_PIN_CNF_DIR_Pos);
  NRF_P0->PIN_CNF[txd]|=(GPIO_PIN_CNF_DIR_Output<<GPIO_PIN_CNF_DIR_Pos);
  NRF_UART0->PSEL.RXD=(rxd<<UART_PSEL_RXD_PIN_Pos)|(UART_PSEL_RXD_CONNECT_Connected<<UART_PSEL_RXD_CONNECT_Pos);
  NRF_UART0->PSEL.TXD=(txd<<UART_PSEL_TXD_PIN_Pos)|(UART_PSEL_TXD_CONNECT_Connected<<UART_PSEL_TXD_CONNECT_Pos);
  NRF_CLOCK->TASKS_HFCLKSTART=1;
  while(NRF_CLOCK->EVENTS_HFCLKSTARTED==0){};
  int mask=(1UL<<16);
  if(NRF_CLOCK->EVENTS_HFCLKSTARTED==1){printf("oscillator started\n");}
  int clk_src=(NRF_CLOCK->HFCLKSTAT& 0b1);
  printf("closk source %d\n",clk_src);
  while((NRF_CLOCK->HFCLKSTAT &mask)==0){printf("waiting for clock to run\n");};
  NRF_UART0->CONFIG|=(UART_CONFIG_HWFC_Disabled<<UART_CONFIG_HWFC_Pos)|(UART_CONFIG_PARITY_Excluded<<UART_CONFIG_PARITY_Pos)|
                          (UART_CONFIG_STOP_One<<UART_CONFIG_STOP_Pos);

  NRF_UART0->BAUDRATE=(UART_BAUDRATE_BAUDRATE_Baud115200<<UART_BAUDRATE_BAUDRATE_Pos);//115200 baudrate
  NRF_UART0->INTENSET=(UART_INTENSET_RXDRDY_Set<<UART_INTENSET_RXDRDY_Pos);
  NRF_UART0->ENABLE=(UART_ENABLE_ENABLE_Enabled<<UART_ENABLE_ENABLE_Pos); //enable UARTE
}

 void uart_write(char* write_buffer){
  NRF_UART0->TASKS_STARTTX=1;
  write_buffer[read_size]=10;
  for(int i=0;i<=read_size;i++){
   NRF_UART0->TXD=(write_buffer[i]);
   while(NRF_UART0->EVENTS_TXDRDY==0){};
   NRF_UART0->EVENTS_TXDRDY=0;
  }
 }




 void uart_read(uint32_t read_buffer[]){
   NRF_UART0->TASKS_STARTRX=1;
   while(NRF_UART0->EVENTS_RXDRDY==0){}
    NRF_UART0->EVENTS_RXDRDY=0;
    int i=0;
    while(i<4){
     read_buffer[i]=NRF_UART0->RXD;
     if(read_buffer[i]=='\r'){
       NRF_UART0->TASKS_STOPRX=1;
       read_size=i;
        break;
      }
     i++;
    while(NRF_UART0->EVENTS_RXDRDY==0){}
    NRF_UART0->EVENTS_RXDRDY=0;
    }

 }

