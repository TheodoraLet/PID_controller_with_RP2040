#ifndef __GPIO_FUNCTIONS_H
#define __GPIO_FUNCTIONS_H

typedef unsigned int uint;

void pinMode(uint pin_number,uint state);

void pin_write(uint pin_number,uint state);

uint pin_read(uint pin_number);

#endif // __GPIO_FUNCTONS_H