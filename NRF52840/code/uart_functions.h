#ifndef __UART_FUNCTIONS_H
#define __UART_FUNCTIONS_H

//void uart_pins(int rxd,int txd);

void uart_config(int rxd,int txd);

void uart_write(uint8_t write_buffer[],uint8_t buffer_size);

//uint32_t uart_read(uint32_t read_buffer[3])

#endif // __UART_FUNCTONS_H