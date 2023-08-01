/*
# ##############################################################################
# File: it.c                                                                   #
# Project: src                                                                 #
# Created Date: Sunday, June 4th 2023, 7:25:27 pm                              #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Saturday, July 29th 2023, 7:59:45 pm                          #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
# Date      	By          	Comments                                       #
# ##########	########    	################################################
 */


#include "main.h"

/*Externs*/
extern CAN_HandleTypeDef hcan1;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}

void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}
void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);	
}
void CAN1_RX1_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}
void CAN1_SCE_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}