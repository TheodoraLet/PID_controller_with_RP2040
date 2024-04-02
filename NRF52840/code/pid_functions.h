#ifndef __PID_FUNCTIONS_H
#define __PID_FUNCTIONS_H

extern volatile int pos;
extern int pin1;
extern int pin2;
extern int pwm_pin;
extern int target_angle;
extern int ENC_PIN1;
extern int ENC_PIN2;

void pid_calculations(void);


void Motor_Drive(void);


#endif // __PID_FUNCTONS_H