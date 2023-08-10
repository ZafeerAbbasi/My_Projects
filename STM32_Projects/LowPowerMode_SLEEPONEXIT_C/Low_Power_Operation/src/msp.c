/*
# ##############################################################################
# File: msp.c                                                                  #
# Project: src                                                                 #
# Created Date: Sunday, June 4th 2023, 7:25:31 pm                              #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Monday, August 7th 2023, 7:35:10 pm                           #
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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	/*1. Enable clock access to TIM6*/
	__HAL_RCC_TIM6_CLK_ENABLE();

	/*Enable IRQ otf TIM6*/
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	/*Set up priority for TIM6_DAC_IRQn*/
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;

	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_SLEEP_DISABLE();
	
	gpio_uart.Pin = GPIO_PIN_8; //USART3 TX AND USART3 RX
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOD, &gpio_uart);
	
	//Enable the IRQ and set up the priority
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
	
}
