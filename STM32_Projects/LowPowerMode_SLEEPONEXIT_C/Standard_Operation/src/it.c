/*
# ##############################################################################
# File: it.c                                                                   #
# Project: src                                                                 #
# Created Date: Sunday, June 4th 2023, 7:25:27 pm                              #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Monday, August 7th 2023, 7:51:28 pm                           #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
# Date      	By          	Comments                                       #
# ##########	########    	################################################
 */


#include "main.h"

extern TIM_HandleTypeDef htimer6;
extern UART_HandleTypeDef huart3;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}


void TIM6_DAC_IRQHandler(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
	HAL_TIM_IRQHandler(&htimer6);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
}


void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart3);
}