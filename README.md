I used a 12V motor with an optical encoder attached to it so i can control the shaft angle. 
The microcontroller i used is Raspberry Pi Pico (RP2040) and the motor driver is TB6612FNG.
The project provides a UART communication interface where the user can type the desired angle in real time and the
desired gains of the PID controller. During the control all data such the response of the PID and the PID error are logged
in a txt file using the SPI protocol with a microSD card connected to Pico. The whole procedure is shown in a video if you follow the drive link.
[[Link of Video](https://drive.google.com/file/d/18O10WC9Mca0Gb-_KqWsRjSpZusg_ET0w/view?usp=drive_link)] (choose 720p quality).
![pico](https://github.com/TheodoraLet/PID_controller_with_RP2040/assets/145222991/00bc40ce-9b28-4a7b-81ac-9c78414ecbc2)
