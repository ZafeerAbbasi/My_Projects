# CAN Communication 


## Project Description
In this project, I developed C code from scratch to implement simple CAN Communication between 2 MCU's: NUCLEO-F446ZE and STM32F103C6T6 BluePill.

## Demo


https://github.com/ZafeerAbbasi/My-Projects/assets/86879362/a03ae9bd-d037-43db-8cca-5d591af6f2ef





## Features
- Implemented button debouncing techniques for reliable behavior from input buttons
- Implemented 50-52MHz clock speed using PLL and HSE for both MCU's
- Implemented status updates and error handling through UART connection
- Implemented CAN Message filtering using ID Mask Mode
- Implemented interrupts for most interactions

## Hardware
- NUCLEO-F446ZE
- STM32F103C6T6 BluePill
- Waveshare SN65HVD230 CAN Board x2
- DSD TECH SH-U09C5 USB to TTL UART Converter
- CANADUINO® ST-Link V2 USB 
- Multi-color LED's x4
- Push buttons x4
- 10KOhm Resistor x4


## Frameworks 
- STM32Cube HAL API's
- PlatformIO

## How it works
1. There are buttons connected to GPIO Pins of the F446ZE, whenver one of these buttons are pressed, they toggle an interrupt which signals the MCU to call a callback function.
2. In the callback, the button that caused the interrupt is determined and then a CAN Data frame is created containing information about the corresponding button and is given a CAN ID.
3. The CAN Frame is then sent by the STM32 bxCAN Peripheral CAN RX and CAN TX pins to the SN65HVD230 Transceiver, which converts the single-ended signals to differential, CANH and CANL signals, and sends it via the CAN Bus to the F103C6.
4. After the CAN Frame is sent, a TxMailboxComplete interrupt is signaled and thus triggers a callback. The STM32 bxCAN has 3 Tx Mailboxes, and I have added a TxMailboxComplete interrupt for each mailbox. So if the Tx scheduler uses mailbox X then TxMailbox(X)Complete interrupt is toggled.
5. In the TxMailboxComplete callback, a status update is sent via UART, as seen in the demo, containing the status of the message (sent or unsent) and the mailbox used to send the message.
6. When the CAN Frame reaches the F103C6, it first goes through a simple CAN Mask Mode filter to see if the CAN ID of the frame matches what we are expecting. (A possible improvement is to add multiple nodes, and each node would only accept CAN Frames for a specific CAN ID. Thus each node would have one LED, and only the node whos filter accepts the pending CAN ID would continue).
7. If the incoming message passes the filter then its placed in the FIFO (First In First Out), if it doesnt pass the filter then it's discarded.
8. Once the message passes the filter and is placed in the FIFO, it toggles a RxFIFOMsgPending interrupt, which signals the MCU that theres a message in the FIFO thats waiting to be read, and in the subsequent callback function, I read the message in the FIFO and abstract the data that contains information about the button pressed.
9. Also in the callback function (RxFIFOMsgPendingCallback), I send a status update via UART, as seen in the demo, containing the status of the recieved message, and the data within the message.
10. Depending on the data (which contains the button pressed), I toggle an a GPIO correspond to the button pressed.
11. Lastly, the toggled GPIO's are connected to LED's, so toggling the GPIO is essentially toggling the LED.

## [Circuit Diagram](https://github.com/ZafeerAbbasi/My-Projects/files/12224016/CAN_COMMUNICATION_V1_CIRCUIT_DIAGRAM_bb.pdf)
![CAN_COMMUNICATION_V1_CIRCUIT_DIAGRAM_bb](https://github.com/ZafeerAbbasi/My-Projects/assets/86879362/60744121-461c-4f35-a804-d3631f504a20)
