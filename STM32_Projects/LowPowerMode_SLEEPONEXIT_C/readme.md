# Low Power Operation using SLEEPONEXIT in STM32 Microcontroller Units (MCUs)

## Introduction
In the dynamic and innovative world of technology, Microcontroller Units (MCUs) play a pivotal role. Renowned for their versatility and multifunctionality, MCUs are embedded into various electronic devices, orchestrating a myriad of tasks, from automation control to mobile radios, and everything in between.

One of the significant attributes of MCUs is their adaptability to operate in low power modes. With the surge in demand for energy-efficient electronics and IoT (Internet of Things) devices, there is an increased emphasis on reducing power consumption to prolong battery life and lower operational costs. For instance, a device that optimizes its energy use will likely perform for extended periods on a single power source, rendering it more efficient and reliable.

The STM32 family of MCUs, popular in the realm of embedded systems, provides various low power modes. One notable feature in this context is the SLEEPONEXIT attribute, which can be harnessed to reduce the current consumption significantly. Our project focuses on exploring this feature in an embedded software application.

## Project Description
This project intends to create an application using an STM32 MCU, specifically focusing on the implementation and examination of its low power sleep mode. The core operation revolves around configuring Timer 6 (TIM6) to trigger an Update interrupt every 10 milliseconds. This interrupt will then invoke a service routine to transmit data via the UART2 interface.

The crucial aspect of our project lies in measuring the current consumption in two distinct scenarios - one with the standard operation (without sleep mode) and the other with the device set to sleep mode using the SLEEPONEXIT feature.

### Methodology
1. **Standard Operation**: Initially, we will establish the MCU's operation without employing any low power modes. The focus will be on setting up TIM6 to trigger an Update interrupt every 10ms, which will, in turn, initiate a routine to send data via UART2. The current consumption in this regular operational mode will be measured and recorded for reference.

2. **Low Power Operation**: In this phase, we will incorporate the SLEEPONEXIT feature, thereby instructing the MCU to enter sleep mode upon exiting the last executed interrupt routine. The same application will run as in the previous phase, but this time with the MCU entering sleep mode when idle, thus minimizing power consumption. The current consumption in this low power mode will be measured and contrasted with the previous results.

## Conclusion
By meticulously exploring the SLEEPONEXIT feature's efficiency, this project aims to highlight the STM32 MCU's potential for low power operation. The results obtained will provide a clear and practical perspective on how energy consumption can be reduced in embedded systems without compromising the application's performance. This endeavor could be a substantial stride towards creating energy-efficient, cost-effective, and more sustainable embedded systems.