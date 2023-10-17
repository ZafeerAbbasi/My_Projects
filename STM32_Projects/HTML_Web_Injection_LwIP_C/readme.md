# HTML Web Injection using LwIP, SSI and CGI with STM32 Board via Ethernet

## Project Description
This project intends to create an application using an STM32 MCU. This application hosts HTTPD server using LwIP which allows the user to monitor and control the hardware from the remote interface.

- Platfrom: PlatformIO on VSCode
- Hardware: NUCLEO-F746ZG Board
- TCP/IP Stack: LwIP
- Webserver Technologies: SSI, CGI

## Demonstration

## Key Features/Accomplishments:
- Wrote all low-level drivers from scratch for the following features:
    - Interfacing LAN8742A PHY Chip to establish Ethernet Connection
    - System Clock driver to run @ 200MHz using PLL sourced from HSE
    - LED Driver
    - USART Driver
    - ADC Driver for monitoring Internal MCU Temperature
    - Timer Driver for generating PWM Signal
- Wrote SSI Handler using LwIP for HTML Web Injection to display real-time information such as Internal Temperature, Elapsed Time, Push Button State and LED states.
- Wrote CGI Handler using LwIP to process CGI Requests from HTTPD Server from user interactions in order to configure hardware as per remote configuration.
- Used DHCP from LwIP to dynamically assign IP Address to the board
- Wrote HTML code for the application, including the buttons, Checkboxes, Sliders etc.


## Whats Going On?
Using LwIP ( TCP/IP Stack ) I'm able to host a web server ( HTTPD ) with my MCU Board. With the help of SSI, I'm able to replace certain tags within the HTML code with values that are sourced from the MCU, for example, Internal MCU Temperature from the ADC, Elapsed Time, and Push Button state. 

But how do I control the LED's from the remote server?

This is done using CGI. CGI allows me get data from the server and process it so that I can use it in order to accomplish some task, like toggling an LED.

## What did I learn from this?
- How Ethernet Works
- How DHCP 
- How to process incoming Ethernet Packets and prep them for the LwIP stack
- How SSI/CGI works
- New HTML Concepts
- Writing Low-level drivers for Physical Layer Chips




