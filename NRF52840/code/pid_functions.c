#include "nrf52840.h"
#include "pid_functions.h"
#include "math.h"
#include "gpio_functions.h"
#include "pwm_functions.h"
#include "nrf52840_bitfields.h"
#include "stdio.h" 
#include "timer_functions.h"

int pin1=27;
int pin2 =6;
int pwm_pin =8;

volatile int pos=0;
int ENC_PIN2 =7;
int ENC_PIN1=26;

int state=1;

volatile int u=0;
uint16_t buffer[1];
int pos_angle;
volatile int target;
int prev_target;
volatile int target_angle;
int prev_t_angle;
volatile int e=0;
int eprev=0;
int eint=0;
int dedt=0;
int deltaT=0;
volatile int pass;

const volatile double kp=0.05;
const volatile double kd=0.01;
const volatile double ki=0;

int map(int x,int l1,int h1,int l2,int h2){
return (int)(x-l1)*(h2-l2)/(h1-l1) +l2;
}

void pid_calculations(){
//pos_angle=map(pos,0,20400,0,360);
target=map(target_angle,0,360,0,40800);
//prev_target=map(prev_t_angle,0,360,0,40800);
e=pos-target;
deltaT=time_interval(0); // 32bit width
dedt=(e-eprev)/deltaT;
eint+=e*deltaT;
eprev=e;
u=kp*e+kd*dedt+ki*eint;
}



void Motor_Drive(){
int temp =(int)fabs(u);
if(temp>255){
*buffer=255;
}else{
*buffer=temp;
}
if(u<0){
pin_write(pin1,0);
pin_write(pin2,1);
}else if(u>0){
pin_write(pin1,1);
pin_write(pin2,0);
}else{
pin_write(pin1,0);
pin_write(pin2,0);
*buffer=0;
pass=1;
}
pwm_write(pwm_pin,0,buffer);
}


