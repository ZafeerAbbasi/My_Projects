/*
# ##############################################################################
# File: it.c                                                                   #
# Project: src                                                                 #
# Created Date: Thursday, July 27th 2023, 10:03:07 pm                          #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Saturday, July 29th 2023, 1:39:31 am                          #
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