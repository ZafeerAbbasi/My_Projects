## Switch Controlled DC Fan Cooling System using 8051 MCU

## Demo 
Circuit simulation in Proteus

![demo_8051](https://user-images.githubusercontent.com/86879362/212532437-9e031289-e8f6-41c4-9515-b6df10e35463.gif)


## Project Description
In this project, a 8051 MCU is used to control a DC brushless axial flow fan that has a Servo DC motor, and a built-in encoder speed sensor. The MCU being
used is a MikronBoard for 8051 40-pin MCU and is programmed in embedded C and flashed to the MCU using Keil µVision® IDE. The speed of the motor is to be 
transmitted and readable using three 7-segment display components. We are to control the motor speed with a PWM output that needs to be programmed 
depending on the 3 digit set on a dip switch. A proportional controller is also implemented that reads the fan speed from the encoder signal and adjusts 
the output signal to the fan depending on the reading fromthe encoder signal.

We are to use:
1. A servo DC motor

![img1](https://user-images.githubusercontent.com/86879362/212532778-7460f8b9-8931-49ba-a97a-77940863074e.jpg)


2. mikronBoard for 8051 40-pin


![img2](https://user-images.githubusercontent.com/86879362/212532783-69f76e3f-ae53-49ed-80e3-7aaa78b0cc26.jpg)

3. A DIP switch
4. Three 7-segment display
5. Resistors: 470 Ω, 1 KΩ, 10 kΩ, 20 kΩ
6. MOSFET Transistor


## Design Process
1. Schematic of the Servo motor control using 8051 MCU
![img3](https://user-images.githubusercontent.com/86879362/212532856-e43f249d-4292-4c44-8084-87105a3dfebf.jpg)

2. MOSFET schematic connections
![img4](https://user-images.githubusercontent.com/86879362/212532874-4f9a38aa-4de9-44fc-94be-11f10f1c8549.jpg)


## Assembled Circuit
![img5](https://user-images.githubusercontent.com/86879362/212533107-854ce0c3-1ed2-4031-b0a8-79ef60155c30.jpg)
![img6](https://user-images.githubusercontent.com/86879362/212533115-f290db8f-d423-41e5-b92a-791ff920772e.jpg)

