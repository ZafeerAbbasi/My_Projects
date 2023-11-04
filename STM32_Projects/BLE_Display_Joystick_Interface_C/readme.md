# Interactivge BLE-Controlled Display and Joystick Interface System

## Project Description
This project intends to create a BLE - Controlled Interface System in which the user can see joystick co-ordinates remotely on a phone via BLE. The user can also display on the LCD Remotely from his phone via BLE.

- Platform: PlatformIO on VSCode
- Libraries/API's:
    - STM32 HAL API
    - STM32 BlueNRG BLE Stack API
- Hardware: 
    - STM32 NUCLEO-F411RE MCU
    - X-NUCLEO-BNRG2A1 BLE Expansion Board
    - LCD 1602 with I2C Bus Expander Module
    - JoyStick Module

## Demonstration
https://github.com/ZafeerAbbasi/My_Projects/assets/86879362/b4887ceb-ddc0-499a-9582-e8cedb03cc94


## Key Features/Accomplishments:
- Wrote all low-level drivers from scratch for the following:
    - ADC to measure JoyStick Readings
    - I2C to display data to the LCD
- Used BLE Stack to create a Service with two main characteristics:
    - 1. Notify User with Updated X,Y JoyStick Co-ordinates
    - 2. Allow the user to write and change the Attribute value


## PowerPoint Slides to teach you about BLE
Here are some simplified slides to teach you the basics about BLE. These slides teach BLE in a way I wished I was learned them when I had started with BLE.
[BLE Powerpoint Slides](BLE.pptx)


## Whats Going On?
The Main Microcontroller on the NUCLEO-F411RE is acting as a BLE Server, and the phone is the BLE Client.
The ADC Peripheral of the MCU is converting Joystick Voltage Readings into digital readings, then uses BLE to send that data to the phone.
I2C is used to send Display data to the LCD, the display data will be received from the phone via BLE.


## What did I learn from this?
- The BLE Stack ( Application, GAP, SMP, GATT, ATT, L2CAP, HCI, LL, PHY )
- Services, Characteristics, UUID, Attributes, Handles, Permissions
- How to create/configure/change Services and Characteristics in BLE
- How to use I2C Peripheral with I2C Bus Expanders
- How to configure an LCD using steps/configurations defined in it's datasheet
- How to use ADC Peripheral to read JoyStick Values




