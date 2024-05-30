#include "app_uart.h"
#include "app_error.h"
#include "nrf_uart.h"
#include "nrf_delay.h"
#include "pid_functions.h"
#include "string.h"
extern volatile int target_angle;

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                       /**< UART RX buffer size. */

#define RX_PIN_NUMBER 11 
#define TX_PIN_NUMBER 12 

void uart_event_handler(app_uart_evt_t * p_event)
    {
        static uint8_t data_array[32];
        static uint8_t index = 0;
       
       // event occurs in peripheral
        switch (p_event->evt_type)
        {
            // when byte arrives at FIFO
            case APP_UART_DATA_READY: 
                // store the byte to an array 
                app_uart_get(&data_array[index]); 
                index++;
                // if it is the end of the message
                if (data_array[index - 1] == '\r') 
                {  
                    target_angle=0;
                    for (uint32_t i = 0; i < strlen((const char *)data_array); i++)
                    {
                        target_angle=target_angle*10+data_array[i]-'0';
                        //transmit the whole message -->print it to the terminal
                        while (app_uart_put(data_array[i]) != NRF_SUCCESS);
                    }
                    // update the desired angle
                    target_angle-=(data_array[index-1]-'0');
                    target_angle/=10;
                    memset(data_array,0,sizeof(data_array));
                    index = 0;
                }
                break;

            case APP_UART_COMMUNICATION_ERROR:
                APP_ERROR_HANDLER(p_event->data.error_communication);
                break;

            case APP_UART_FIFO_ERROR:
                APP_ERROR_HANDLER(p_event->data.error_code);
                break;

            default:
                break;
        }
    }


void uart_init(void)
 {
   uint32_t err_code;
   //initialie the uart instance
 const app_uart_comm_params_t comm_params = 
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          NULL,
          NULL,
         APP_UART_FLOW_CONTROL_DISABLED, // no flow control
          false,                         // no parity bit
         NRF_UART_BAUDRATE_115200        // 115200 baud rate
      };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_event_handler,              
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);

    APP_ERROR_CHECK(err_code);

 }


