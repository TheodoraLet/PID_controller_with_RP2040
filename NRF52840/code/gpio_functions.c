#include "nrf52840.h"
#include "gpio_functions.h"

void pinMode(uint pin_number,uint state){
if(state==1){ // 1= output
NRF_P0->DIRSET=(1U<<pin_number);
}else{ // 0=input
NRF_P0->DIRCLR=(1U<<pin_number);
}
}

void pin_write(uint pin_number,uint state){
if(state==1){ // 1=HIGH
NRF_P0->OUTSET=(1UL<<pin_number);
}else{ // 0=LOW
NRF_P0->OUTCLR=(1UL<<pin_number);
}
}

uint pin_read(uint pin_number){ // only for input pins
uint mask=(1UL<<pin_number);
uint res=NRF_P0->IN & mask;
return res;
}





