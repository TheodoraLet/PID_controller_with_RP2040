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
volatile int deltaT=0;

const volatile double kp=0.01;
const volatile double kd=0.1;
const volatile double ki=0;

int map(int x,int l1,int h1,int l2,int h2){
 return (int)(x-l1)*(h2-l2)/(h1-l1) +l2;
}

void pid_calculations(){
  // map angle to pulses
  target=map(target_angle,0,360,0,40800); 
  // calculate error
  e=pos-target;       
  // time between two consecutive controls                 
  deltaT=time_interval(0);             
  dedt=(e-eprev)/deltaT;   //calculate
  eint+=e*deltaT;         // PID errors 
  // store previous error for "dedt" calculation 
  eprev=e;                         
  // update control signal
  u=kp*e+kd*dedt+ki*eint;           
}



void Motor_Drive(){
  int temp =(int)fabs(u);
  if(temp>255){    
  // update duty cycle
    *buffer=255; 
  // maintaining the limits     
  }else{           
    *buffer=temp;   
  }
  // for negative control signal 
  if(u<0){   
    // rotate anticlockwise        
    pin_write(pin1,0); 
    pin_write(pin2,1);
  // for positive control signal
  }else if(u>0){     
  // rotate clockwise  
    pin_write(pin1,1); 
    pin_write(pin2,0);
// when control signal equals 0
  }else{  
 // stop rotation
    pin_write(pin1,0); 
    pin_write(pin2,0);
    *buffer=0;
  }
  // send correct duty cycle 
  pwm_write(pwm_pin,0,buffer); 
}


