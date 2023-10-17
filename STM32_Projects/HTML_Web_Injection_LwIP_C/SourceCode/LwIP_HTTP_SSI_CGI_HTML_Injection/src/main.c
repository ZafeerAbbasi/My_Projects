/*
# ##############################################################################
# File: main.c                                                                 #
# Project: src                                                                 #
# Created Date: Tuesday, September 26th 2023, 7:58:59 pm                       #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, October 15th 2023, 6:40:27 pm                         #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */

/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include <stdint.h>
#include "lwipopts.h"
#include "main.h"
#include "usr00_led.h"
#include "usr01_uart.h"
#include "usr02_adc.h"
#include "usr03_ssi_process.h"
#include "usr05_cgi_process.h"
#include "usr06_timer.h"
#include "lwip.h"

/*##############################################################################################################################################*/
/*FUNCTION DECLARATIONS_________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/


 
/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

uint32_t HCLKFreq, D2PCLK1Freq, D2PCLK2Freq, D1PCLK3Freq, D3PCLK4Freq, SYSCLKFreq, PLL2PFreq;
float sensorVal;

/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/




/*##############################################################################################################################################*/
/*EXTERNS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

extern ADC_HandleTypeDef hADC1;
extern struct netif gNetif;
extern char const ** tags;
extern const tCGI CGI_LED;
extern const tCGI CGI_SLIDER;
extern tCGI ARR_CGI[ NUM_OF_CGI ];


/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

int main(void)
{
    asm(".global _printf_float");
	
	HAL_Init( );
	CLOCK_sysClockConfig( );

	LED_Init( );
	UART_usart3TXInit( );
	ADC_ADC12INP3PA6Init( );
	TIM_Timer5Init( );

	LWIP_libInit( );
	httpd_init( );
	http_set_ssi_handler( SSI_SSIHandler, ( const char ** )tags, NUM_OF_TAGS );
	
	ARR_CGI[ 0 ] = CGI_LED;
	ARR_CGI[ 1 ] = CGI_SLIDER;
	http_set_cgi_handlers( ARR_CGI, NUM_OF_CGI );
	
	HCLKFreq = HAL_RCC_GetHCLKFreq( );
	D2PCLK1Freq = HAL_RCC_GetPCLK1Freq( );
	D2PCLK2Freq = HAL_RCC_GetPCLK2Freq( );
	SYSCLKFreq = HAL_RCC_GetSysClockFreq( );
	

	LED_deactivateLED( GREEN_LED );
	LED_deactivateLED( BLUE_LED );
	LED_deactivateLED( RED_LED );
	
	notifyConnChanged( &gNetif );
	while(1)
	{
		LWIP_process( );
		sensorVal = ADC_getMCUTemp( );
		printf("IP Address from DHCP:  %s\r\n", ip4addr_ntoa( &gNetif.ip_addr ) );
	}
}

/**
 * @brief System Clock Config
 * 
 */
void CLOCK_sysClockConfig( void )
{
	RCC_OscInitTypeDef	RCC_oscInitStruct = { 0 };
	RCC_ClkInitTypeDef	RCC_clkInitStruct = { 0 };
	__HAL_RCC_PWR_CLK_ENABLE( );

	/*Set Voltage Scale to 1, in order to set to 0, must be in 1 first*/
	__HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );

	/*OSCILLATOR CONFIG----------------------------------------------------------------------------*/
	/*SYSCLK @ 200MHz*/
	RCC_oscInitStruct.OscillatorType	= RCC_OSCILLATORTYPE_HSE;
	RCC_oscInitStruct.HSEState			= RCC_HSE_BYPASS;
	
	/*OSCILLATOR PLL CONFIG*/
	RCC_oscInitStruct.PLL.PLLState		= RCC_PLL_ON;
	RCC_oscInitStruct.PLL.PLLSource		= RCC_PLLSOURCE_HSE;
	RCC_oscInitStruct.PLL.PLLM			= 4;
	RCC_oscInitStruct.PLL.PLLN			= 200;
	RCC_oscInitStruct.PLL.PLLP			= RCC_PLLP_DIV2;
	RCC_oscInitStruct.PLL.PLLQ			= 2;

	if( HAL_RCC_OscConfig( &RCC_oscInitStruct ) != HAL_OK )
	{
		ERROR_errorHandler( );
	}

	/*Activate overdrive*/
	HAL_PWREx_EnableOverDrive( );

	/*CLOCK CONFIG---------------------------------------------------------------------------------*/
	RCC_clkInitStruct.ClockType 		= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
										  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_clkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_clkInitStruct.AHBCLKDivider		= RCC_SYSCLK_DIV1;
	RCC_clkInitStruct.APB1CLKDivider	= RCC_HCLK_DIV4;
	RCC_clkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV4;
	
	if( HAL_RCC_ClockConfig( &RCC_clkInitStruct, FLASH_ACR_LATENCY_5WS ) != HAL_OK )
	{
		ERROR_errorHandler( );
	}
	
}

/**
 * @brief Error Handler
 * 
 */
void ERROR_errorHandler( void )
{
	while( 1 ) { 
		//printf("Error");
	}
}

/**
 * @brief Ethernet Connection Change Notification Callback Implementation
 * 
 * @param netif 
 */
void notifyConnChanged( struct netif *netif )
{
	if( netif_is_link_up( netif ) )
	{
		printf( "Cable is Connected!!! \r\n ");
	}
	else
	{
		printf(" ERR : Cable not connected !!\r\n ");
	}
}

