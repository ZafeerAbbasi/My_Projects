/*
# ##############################################################################
# File: usr02_adc.c                                                            #
# Project: src                                                                 #
# Created Date: Saturday, September 30th 2023, 8:01:17 pm                      #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Saturday, October 14th 2023, 8:07:18 am                       #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */

/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "usr02_adc.h"

/*##############################################################################################################################################*/
/*FUNCTION DECLARATIONS_________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/*ADC Handle Typedef*/
ADC_HandleTypeDef hADC1;

/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#define SENSOR_PIN		(1U<<4)
#define SENSOR_PORT		GPIOA

/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/**
 * @brief ADC Config for ADC12 INP3; ADC1/2 Channel 3 Input Positive Single Ended
 * 
 */
void ADC_ADC12INP3PA6Init( void )
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
    ADC_ChannelConfTypeDef sConfig 	  = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_ADC1_CLK_ENABLE();

	/*Set PA4 as analog pin*/
	GPIO_InitStruct.Pin  = SENSOR_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(SENSOR_PORT,&GPIO_InitStruct);


	/*Configure ADC parameters*/

	hADC1.Instance =  ADC1;
	hADC1.Init.ClockPrescaler =  ADC_CLOCK_SYNC_PCLK_DIV8;
	hADC1.Init.Resolution	  =  ADC_RESOLUTION_12B;
	hADC1.Init.ScanConvMode   =  ADC_SCAN_DISABLE;
	hADC1.Init.ContinuousConvMode	= DISABLE;
	hADC1.Init.DiscontinuousConvMode = DISABLE;
	hADC1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hADC1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hADC1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hADC1.Init.NbrOfConversion = 1;
	hADC1.Init.DMAContinuousRequests = DISABLE;
	hADC1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hADC1);

	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	sConfig.Rank    = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime =  ADC_SAMPLETIME_3CYCLES;

	HAL_ADC_ConfigChannel(&hADC1,&sConfig);

}

/**
 * @brief Convert ADC Reading to Voltage
 * 
 * @param ADCValue ADC Reading, 0 - 4096 
 * @return float Voltage
 */
float ADC_ADCToVoltage( int ADCValue )
{
    float maxADCValue = 4095.0;
    return ( ADCValue / maxADCValue ) * 3.3;
}

/**
 * @brief Convert ADC Voltage to Degrees Celsius
 * 
 * @param ADCValue 
 * @return float 
 */
float ADC_ADCToDegrees( int ADCValue )
{
	return ( ( ADC_ADCToVoltage( ADCValue ) * 25 ) / 0.76 );
}

/**
 * @brief Get MCU Temperature
 * 
 * @return float MCU Temperature in Degrees Celsius
 */
float ADC_getMCUTemp( void )
{
	HAL_ADC_Start( &hADC1 );
	HAL_ADC_PollForConversion( &hADC1, HAL_MAX_DELAY );
	HAL_ADC_Stop( &hADC1 );

	float temp = ADC_ADCToDegrees( HAL_ADC_GetValue( &hADC1 ) );
	return temp;
}


/*##############################################################################################################################################*/
/*EXTERNS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/