/*
# ##############################################################################
# File: msp.c                                                                  #
# Project: src                                                                 #
# Created Date: Thursday, July 27th 2023, 10:03:19 pm                          #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Saturday, July 29th 2023, 7:56:44 pm                          #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
# Date      	By          	Comments                                       #
# ##########	########    	################################################
 */


#include "main.h"

void HAL_MspInit(void)
{
	//Here we will do low level processor specific inits.
	//1. Set up the priority grouping of the arm cortex mx processor

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	// ^This line is actually not needed since it would by default assign priority group 4 if nothing is explicitly set

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; // enabled usg fault, memory fault, and bus fault system exceptions


	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	// ^These lines are actually not needed since it would be default assigned a priority group of 0 meaning it has the highest priority
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	/*Enable GPIO Pins for UART Communication*/
	GPIO_InitTypeDef uart_gpio;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();

	uart_gpio.Pin = GPIO_PIN_10 | GPIO_PIN_9;
	uart_gpio.Mode = GPIO_MODE_AF_PP;
	uart_gpio.Pull = GPIO_PULLUP;
	uart_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &uart_gpio);

	/*NVIC IRQ settings*/
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
	
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	/*Creating gpio init typedef for CAN1 gpio pins*/
	GPIO_InitTypeDef can_gpio;
	
	/*Enabling clock for CAN1 peripheral*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_CAN1_CLK_ENABLE();

	/*These settings are for CAN loopback mode, so thats why we have GPIO_MODE_AF_PP and GPIO_NOPULL*/
	can_gpio.Pin = GPIO_PIN_12; //CAN TX
	can_gpio.Mode = GPIO_MODE_AF_PP;
	can_gpio.Pull = GPIO_NOPULL;
	can_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	
	/*Initialize the gpio struct*/
	HAL_GPIO_Init(GPIOA, &can_gpio);

	can_gpio.Pin = GPIO_PIN_11; //CAN RX
	can_gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &can_gpio);

	/*Set Priority for CAN Interrupts*/
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);

	/*Enable the interrupts*/
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

}