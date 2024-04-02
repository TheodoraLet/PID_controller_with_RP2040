I used a 12V motor with an optical encoder attached to it so i can control the shaft angle. 
The microcontrollers i used are Raspberry Pi Pico (RP2040), Teensy 4.1 and Adafruit Feather NRF52840, the motor driver i used is TB6612FNG.
The project provides a UART communication interface where the user can type the desired angle in real time and the
desired gains of the PID controller. During the control all data such the response of the PID and the PID error are logged
in a txt file using the SPI protocol with a microSD card connected to the microcontrollers. 

