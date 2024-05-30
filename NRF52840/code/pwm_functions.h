#ifndef __PWM_FUNCTIONS_H
#define __PWM_FUNCTIONS_H

typedef unsigned int uint;

void pwm_set_format(uint pwm_pin,uint top);

void pwm_write(uint pwm_pin,uint chan,uint16_t buf[1]);

#endif // __PWM_FUNCTONS_H