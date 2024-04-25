#ifndef __UART_FUNCTIONS_H
#define __UART_FUNCTIONS_H

#include "stdint.h"
extern volatile uint8_t read_size;

void uart_write(char write_buffer[]);

void uart_read(uint32_t read_buffer[]);

void uart_config(int rxd,int txd);

#endif // __UART_FUNCTONS_H