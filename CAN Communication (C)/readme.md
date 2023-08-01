## CAN Communication 


## Project Description
In this project, I developed C code from scratch to implement simple CAN Communication between 2 MCU's: NUCLEO-F446ZE and STM32F103C6T6 BluePill.

## Features
- Implemented button debouncing techniques for reliable behavior from input buttons
- Implemented 50-52MHz clock speed using PLL and HSE for both MCU's
- Implemented status updates and error handling through UART connection
- Implemented CAN Message filtering
- Implemented interrupts for most interactions

## Hardware
- NUCLEO-F446ZE
- STM32F103C6T6 BluePill
- Waveshare SN65HVD230 CAN Board x2
- DSD TECH SH-U09C5 USB to TTL UART Converter
- CANADUINO® ST-Link V2 USB 


## Frameworks 
- STM32Cube HAL API's
- PlatformIO

## How it works
1. There are buttons connected to GPIO Pins of the F446ZE, whenver one of these buttons are pressed, they toggle an interrupt which signals the MCU to call a callback function.
2. The callback function saves which button number was pressed and then sends the data via CAN to the F103C6.
3. When the F103C6 received the CAN Data Frame, it toggles a receive interrupt, which signals the MCU to call a callback function.
4. This callback function interprets the CAN Data and then toggles a GPIO corresponding to the CAN data, and the GPIO is connected to an LED.

## [Circuit Diagram](https://github.com/ZafeerAbbasi/My-Projects/files/12224016/CAN_COMMUNICATION_V1_CIRCUIT_DIAGRAM_bb.pdf)
![CAN_COMMUNICATION_V1_CIRCUIT_DIAGRAM_bb](https://github.com/ZafeerAbbasi/My-Projects/assets/86879362/60744121-461c-4f35-a804-d3631f504a20)
