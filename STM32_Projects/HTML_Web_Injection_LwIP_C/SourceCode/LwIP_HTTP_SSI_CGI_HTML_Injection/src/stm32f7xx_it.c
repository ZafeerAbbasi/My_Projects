/*
# ##############################################################################
# File: stm32h7xx_it.c                                                         #
# Project: src                                                                 #
# Created Date: Tuesday, September 26th 2023, 1:37:17 am                       #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, October 15th 2023, 4:54:49 am                         #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */

#include "main.h"
#include "stm32f7xx_it.h"
#include "ethernetif.h"

extern ETH_HandleTypeDef heth;

void SysTick_Handler( void )
{
    HAL_IncTick( );   
}

void ETH_IRQHandler( void )
{
    HAL_ETH_IRQHandler( &heth );
}

