#ifndef __PID_FUNCTIONS_H
#define __PID_FUNCTIONS_H

extern volatile int pos;
extern volatile int u;
extern volatile int pass;
extern int pin1;
extern int pin2;
extern int pwm_pin;
extern volatile int target_angle;
extern int ENC_PIN1;
extern int ENC_PIN2;
extern volatile int e;
extern volatile int target;
extern volatile int deltaT;

void pid_calculations(void);


void Motor_Drive(void);


#endif // __PID_FUNCTONS_H