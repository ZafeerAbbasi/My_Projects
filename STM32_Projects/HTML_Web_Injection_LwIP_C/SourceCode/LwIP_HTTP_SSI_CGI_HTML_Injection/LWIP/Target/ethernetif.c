/*
# ##############################################################################
# File: ethernetif.c                                                           #
# Project: Target                                                              #
# Created Date: Tuesday, September 26th 2023, 2:23:01 am                       #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Thursday, October 12th 2023, 11:49:55 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */


/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "ethernetif.h"

/*##############################################################################################################################################*/
/*FUNCTION DECLARATIONS_________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

static void ETH_LowLevelInit( struct netif *netif );
static err_t ETH_LowLevelTxOutput( struct netif *netif, struct pbuf *LWIPPackBuff );
static struct pbuf *ETH_LowLevelRxInput( struct netif *netif );
static void ETH_ProcessError( void );


/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

__ALIGN_BEGIN  ETH_DMADescTypeDef DMARxDscrTab[ETH_RXBUFNB]  __ALIGN_END;
__ALIGN_BEGIN  ETH_DMADescTypeDef DMATxDscrTab[ETH_TXBUFNB]  __ALIGN_END;
__ALIGN_BEGIN  uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END;
__ALIGN_BEGIN  uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END;

/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#define IFNAME0	'Z'
#define IFNAME1	'A'

/*##############################################################################################################################################*/
/*EXTERNS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

extern ETH_HandleTypeDef heth;

/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/**
 * @brief Init the low level hardware : GPIO, CLOCK, NVIC.
 * 
 * @param heth Pointer to Eth Handle Typedef
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef    GPIO_initStruct = { 0 };

    if( heth->Instance == ETH )
    {
        /*ETH GPIO CONFIGURATION
        PA1--------------> ETH_RMII_REF_CLK
        PA2--------------> ETH_RMII_MDIO
        PA7--------------> ETH_RMII_CRS_DV
        PB13-------------> ETH_RMII_TXD1
        PC1--------------> ETH_RMII_MDC 
        PC4--------------> ETH_RMII_RXD0
        PC5--------------> ETH_RMII_RXD1
        PG11-------------> ETH_RMII_TX_EN
        PG13-------------> ETH_RMII_TXD0
        */

       /*Enable all clocks*/
       __HAL_RCC_GPIOA_CLK_ENABLE( );
       __HAL_RCC_GPIOB_CLK_ENABLE( );
       __HAL_RCC_GPIOC_CLK_ENABLE( );
       __HAL_RCC_GPIOG_CLK_ENABLE( );
       __HAL_RCC_ETH_CLK_ENABLE( );

       /*GPIOA*/
       GPIO_initStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
       GPIO_initStruct.Mode = GPIO_MODE_AF_PP;
       GPIO_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
       GPIO_initStruct.Alternate = GPIO_AF11_ETH;
       HAL_GPIO_Init( GPIOA, &GPIO_initStruct );

       /*GPIOB*/
       GPIO_initStruct.Pin = GPIO_PIN_13;
       GPIO_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
       GPIO_initStruct.Alternate = GPIO_AF11_ETH;
       HAL_GPIO_Init( GPIOB, &GPIO_initStruct );

       /*GPIOC*/
       GPIO_initStruct.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
       GPIO_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
       GPIO_initStruct.Alternate = GPIO_AF11_ETH;
       HAL_GPIO_Init( GPIOC, &GPIO_initStruct );

       /*GPIOG*/
       GPIO_initStruct.Pin = GPIO_PIN_11 | GPIO_PIN_13;
       GPIO_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
       GPIO_initStruct.Alternate = GPIO_AF11_ETH;
       HAL_GPIO_Init( GPIOG, &GPIO_initStruct );

       /*NVIC ( Nested Vector Interrupt Controller ) == Interrupt Settings*/
       HAL_NVIC_SetPriority( ETH_IRQn, 0, 0 );
       HAL_NVIC_EnableIRQ( ETH_IRQn );
    }
}

/**
 * @brief Low Level ETH Init
 * 
 * @param netif LWIP Network Interface Struct
 */
static void ETH_LowLevelInit( struct netif *netif )
{
    uint32_t regValue = 0;
    HAL_StatusTypeDef halETHInitStatus;
    //6 elements x 8 bits per element = 40bits == MAC Address
    uint8_t MACAddr[ 6 ] = {
        0x00,
        0xAB,
        0xCD,
        0x00,
        0x00,
        0x00,
    };

    /*Eth Handle Config*/
    heth.Instance               = ETH;
    heth.Init.MACAddr           = MACAddr;
    heth.Init.AutoNegotiation   = ETH_AUTONEGOTIATION_ENABLE;
    heth.Init.Speed             = ETH_SPEED_100M;
    heth.Init.DuplexMode        = ETH_MODE_FULLDUPLEX;
    heth.Init.PhyAddress        = LAN8742A_PHY_ADDRESS;
    heth.Init.MACAddr           = MACAddr;
    heth.Init.RxMode            = ETH_RXPOLLING_MODE;
    heth.Init.ChecksumMode      = ETH_CHECKSUM_BY_HARDWARE;
    heth.Init.MediaInterface    = ETH_MEDIA_INTERFACE_RMII;
    
    halETHInitStatus = HAL_ETH_Init( &heth );

    if( halETHInitStatus == HAL_OK )
    {
        netif->flags |= NETIF_FLAG_LINK_UP;
    }

    /*Descriptor Init*/
    HAL_ETH_DMATxDescListInit( &heth, DMATxDscrTab, &Tx_Buff[ 0 ][ 0 ], ETH_TXBUFNB );
    HAL_ETH_DMARxDescListInit( &heth, DMARxDscrTab, &Rx_Buff[ 0 ][ 0 ], ETH_RXBUFNB );

    /*MAC Address*/
    netif->hwaddr_len = ETH_HWADDR_LEN;
    netif->hwaddr[ 0 ] = MACAddr[ 0 ];
    netif->hwaddr[ 1 ] = MACAddr[ 1 ];
    netif->hwaddr[ 2 ] = MACAddr[ 2 ];
    netif->hwaddr[ 3 ] = MACAddr[ 3 ];
    netif->hwaddr[ 4 ] = MACAddr[ 4 ];
    netif->hwaddr[ 5 ] = MACAddr[ 5 ];

    /*Set MTU ( Maximum Transfer Unit )*/
    netif->mtu = 1500;

    /*Enable broadcasting and ARP*/
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
    
    /*Start MAC and DMA Transmission and reception*/
    HAL_ETH_Start( &heth );

    /*Enable Link Down interrupt*/
    HAL_ETH_ReadPHYRegister( &heth, PHY_ISFR, &regValue );
    regValue |= PHY_ISFR_INT4;
    HAL_ETH_WritePHYRegister( &heth, PHY_ISFR, regValue );
    HAL_ETH_ReadPHYRegister( &heth, PHY_ISFR, &regValue );
}

/**
 * @brief Ethernet Interface Initialization
 * 
 * @param netif LWIP Network Interface Struct
 * @return err_t Status of Initialization
 */
err_t ETH_EthernetIFInit( struct netif *netif )
{
	/*Set netif name*/
	netif->name[0] =  IFNAME0;
	netif->name[1] =  IFNAME1;

	/*Set netif output function*/
	netif->output = etharp_output;

	/*Set netif link output function*/
	netif->linkoutput =  ETH_LowLevelTxOutput;

	/*Init hardware*/
	ETH_LowLevelInit( netif );

	return ERR_OK;
}

/**
 * @brief Takes Received RX Packet and passes it to LwIP
 * 
 * @param netif LWIP Network Interface Struct
 */
void ETH_EthernetIFInput( struct netif *netif )
{
    err_t err;
    struct pbuf *packBuff;

    /*Move received Packet into new Packet Buff*/
    packBuff = ETH_LowLevelRxInput( netif );

    if( packBuff == NULL )
    {
        return;
    }

    /*Entry Point into LwIP Stack*/
    err = netif->input( packBuff, netif );

    if( err != ERR_OK )
    {
        pbuf_free( packBuff );
        packBuff = NULL;
    }
}

/**
 * @brief Takes a packet from LwIP Stack, prepares and transmits data to Etherenet MAC Peripheral
 * and sends data over the wire using the MAC Peripheral over the Ethernet Interface
 * 
 * @param netif LWIP Network Interface Struct
 * @param p LWIP Packet Buffer
 * @return err_t Status of Transmission
 */
static err_t ETH_LowLevelTxOutput( struct netif *netif, struct pbuf *LWIPPackBuff )
{
    err_t errVal;
    struct pbuf *packBuffTmplte;
    __IO ETH_DMADescTypeDef *DMATxDesc;
    DMATxDesc = heth.TxDesc;
    uint8_t *bufferAddr = ( uint8_t * )( heth.TxDesc->Buffer1Addr );

    uint32_t frameLength        = 0;
    uint32_t bufferOffSet       = 0;
    uint32_t bytesLeftToCopy    = 0;
    uint32_t payLoadOffSet      = 0;

    /*Copy Frames from LWIP Packet Buffer to ETH Buffer*/
    for( packBuffTmplte = LWIPPackBuff; packBuffTmplte != NULL; packBuffTmplte = packBuffTmplte->next )  
    {
        /*Check if ETH Buffer is available*/
        if( ( DMATxDesc->Status & ETH_DMATXDESC_OWN ) != ( uint32_t )RESET )
        {
            /*Descriptor is in use and descriptor is owned by DMA engine */
            errVal = ERR_USE;
            ETH_ProcessError( );
        }

        bytesLeftToCopy = packBuffTmplte->len;
        payLoadOffSet = 0;

        /*Copy and update size variables when data length is larger than TX Buffer size*/
        while( ( bytesLeftToCopy + bufferOffSet ) > ETH_TX_BUF_SIZE )
        {
            /*Copy data to TX Buffer*/
            memcpy( ( uint8_t * )( ( uint8_t * )bufferAddr + bufferOffSet ),
                    ( uint8_t * )( packBuffTmplte->payload + payLoadOffSet ),
                    ( ETH_TX_BUF_SIZE - bufferOffSet ) );
            
            /*Point to the next descriptor*/
            DMATxDesc = ( ETH_DMADescTypeDef * )( DMATxDesc->Buffer2NextDescAddr );

            /*Check if ETH Buffer is available*/
            if( ( DMATxDesc->Status & ETH_DMATXDESC_OWN ) != ( uint32_t )RESET )
            {
                /*Descriptor is in use and descriptor is owned by DMA engine */
                errVal = ERR_USE;
                ETH_ProcessError( );
            }

            /*Update buffer and variables*/
            bufferAddr      = ( uint8_t * )( DMATxDesc->Buffer1Addr );
            bytesLeftToCopy = bytesLeftToCopy - ( ETH_TX_BUF_SIZE - bufferOffSet );
            payLoadOffSet   = payLoadOffSet + ( ETH_TX_BUF_SIZE - bufferOffSet );
            frameLength     = frameLength + ( ETH_TX_BUF_SIZE - bufferOffSet );
            bufferOffSet    = 0;
        }

        /*Remaing bytes are less than size of ETH TX BUF SIZE, so copy rest of bytes*/
        memcpy( ( uint8_t * )( ( uint8_t * )bufferAddr + bufferOffSet ),
                ( uint8_t * )( packBuffTmplte->payload + payLoadOffSet ),
                bytesLeftToCopy );
        bufferOffSet    = bufferOffSet + bytesLeftToCopy;
        frameLength     = frameLength + bytesLeftToCopy;
    }  

    /*Copying from LWIP Packet Buffer to ETH Descriptors is done*/
    /*Prep TX Descriptors and give it DMA which will Transmit it*/
    HAL_ETH_TransmitFrame( &heth, frameLength );
    errVal = ERR_OK;

    /*Check if Transmit UnderFlow Status ( TUS ) is set*/
    if( ( heth.Instance->DMASR & ETH_DMASR_TUS ) != ( uint32_t )RESET )
    {
        /*Clear Transmit UnderFlow Status*/
        /*From STM32F746ZG Ref Manual:
        Writing 1 to (unreserved) bits in ETH_DMASR register[16:0] clears them and writing 0 has no effect*/
        heth.Instance->DMASR = ETH_DMASR_TUS;

        /*Resume DMA Transmission*/
        heth.Instance->DMATPDR = 0;
    }

    return errVal;
}

/**
 * @brief Receives Ethernet frames from the Ethernet MAC peripheral which is fetching incoming data packets from DMA
 * processes them, and then passes them to the higher-level LwIP stack for further handling. 
 * 
 * @param netif LWIP Network Interface Struct
 * @return struct pbuf* Pointer to the Packet Buffer ready for LwIP
 */
static struct pbuf *ETH_LowLevelRxInput( struct netif *netif )
{
    __IO ETH_DMADescTypeDef *DMARxDesc;
    uint8_t *RxBuffAddr;
    struct pbuf *packBuff       = NULL;
    struct pbuf *packBuffTmplt  = NULL;
    uint32_t RxBuffLen          = 0;
    uint32_t buffOffSet         = 0;
    uint32_t payLoadOffSet      = 0;
    uint32_t bytesLeftToCopy    = 0;

    /*Get Received Frame from DMA Descriptor*/
    if( HAL_ETH_GetReceivedFrame( &heth ) != HAL_OK )
    {
        return NULL;
    }

    /*Get Address and length of received packet*/
    RxBuffAddr = (uint8_t * )heth.RxFrameInfos.buffer;
    RxBuffLen = ( heth.RxFrameInfos.length );

    if( RxBuffLen > 0 )
    {
        /*Allocate Packet Buffer from LwIP buffer pool*/
        packBuff = pbuf_alloc( PBUF_RAW, RxBuffLen, PBUF_POOL );
    }
    
    if( packBuff != NULL )
    {
        DMARxDesc = heth.RxFrameInfos.FSRxDesc;
        buffOffSet = 0;
        
        /*Copy Frames from ETH Buffer to Lwip Packet Buffer*/
        for( packBuffTmplt = packBuff; packBuffTmplt != NULL; packBuffTmplt = packBuffTmplt->next )
        {
            bytesLeftToCopy = packBuffTmplt->len;
            payLoadOffSet = 0;

            /*Copy and update size variables when data length is larger than RX BUFF Size*/
            while( ( bytesLeftToCopy + buffOffSet ) > ETH_RX_BUF_SIZE )
            {
                /*Copy ETH RX Data to Packet Buffer*/
                memcpy( ( uint8_t * )( ( uint8_t * )packBuffTmplt->payload + payLoadOffSet ),
                        ( uint8_t * )( ( uint8_t * )RxBuffAddr + buffOffSet ),
                        ( ETH_RX_BUF_SIZE - buffOffSet ) );

                /*Point to the next descriptor*/
                DMARxDesc = ( ETH_DMADescTypeDef *)( DMARxDesc->Buffer2NextDescAddr );

                /*Update variables*/
                RxBuffAddr          = ( uint8_t * )( DMARxDesc->Buffer1Addr );
                bytesLeftToCopy     = bytesLeftToCopy - ( ETH_RX_BUF_SIZE - buffOffSet );
                payLoadOffSet       = payLoadOffSet - ( ETH_RX_BUF_SIZE - buffOffSet );
            }

            /*Remaing bytes are less than size of ETH TX BUF SIZE, so copy rest of bytes*/
            memcpy( ( uint8_t * )( ( uint8_t * )packBuffTmplt->payload + payLoadOffSet ),
                    ( uint8_t * )( ( uint8_t * )RxBuffAddr + buffOffSet ),
                    ( bytesLeftToCopy) );

            buffOffSet = buffOffSet + bytesLeftToCopy;
        }
    }

    /*Point to the first descriptor*/
    DMARxDesc = heth.RxFrameInfos.FSRxDesc;

    /*Set OWN Bit in RX Descriptors ( Handing it back to DMA Engine )*/
    for( int i = 0; i < heth.RxFrameInfos.SegCount; i++ )
    {
        DMARxDesc->Status |= ETH_DMARXDESC_OWN;
        DMARxDesc = ( ETH_DMADescTypeDef * )( DMARxDesc->Buffer2NextDescAddr );
    }

    /*Clear segment count ( Segment Count = Number of RX Descriptors )*/
    heth.RxFrameInfos.SegCount = 0;

    /*Check if RX Buffer unavailable flag is SET
    If SET, RESET bit and resume reception*/
    if( ( heth.Instance->DMASR & ETH_DMASR_RBUS ) != ( uint32_t )( RESET ) )
    {
        /*Clear RBUS Flag*/
        heth.Instance->DMASR = ETH_DMASR_RBUS;

        /*Resume Reception*/
        heth.Instance->DMARPDR = 0;
    }

    return packBuff;
}

/**
 * @brief Process Error ( UnderFlow Error )
 * 
 */
static void ETH_ProcessError( void )
{
    
    /*What is an UnderFlow Error?
    In the context of Direct Memory Access (DMA) and Ethernet, an "underflow error" typically occurs 
    when the DMA controller is unable to read data from the memory fast enough to keep up with the 
    speed at which the Ethernet MAC (Media Access Control) is transmitting data. Essentially, the 
    transmit buffer "underflows" because it becomes empty before the DMA can refill it*/
    
    /*Check if Transmit UnderFlow Status ( TUS ) is set*/
    if( ( heth.Instance->DMASR & ETH_DMASR_TUS ) != ( uint32_t )RESET )
    {
        /*Clear Transmit UnderFlow Status*/
        /*From STM32F746ZG Ref Manual:
        Writing 1 to (unreserved) bits in ETH_DMASR register[16:0] clears them and writing 0 has no effect*/
        heth.Instance->DMASR = ETH_DMASR_TUS;

        /*Resume DMA Transmission*/
        heth.Instance->DMATPDR = 0;
    }
}

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1, used in LwIP
* @param  None
* @retval Current Time value
*/
u32_t sys_now( void )
{
  return HAL_GetTick( );
}

/**
 * @brief This is called whenever the link is changed, it then updates the config to 
 * the new changes
 * 
 * @param netif LWIP Network Interface Struct
 */
void ETH_EthernetIFUpdateConfig( struct netif *netif )
{
    __IO uint32_t tickStart = 0;
    uint32_t regVal         = 0;

    /*Check if Link is up*/
    if( netif_is_link_up( netif ) )
    {
        /*Link is up, check if auto-negotiation is enabled*/
        if( heth.Init.AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE )
        {
            /*Autonegotiation is disabled, so enable it*/
            HAL_ETH_WritePHYRegister( &heth, PHY_BCR, PHY_AUTONEGOTIATION );

            /*Get current tick value*/
            tickStart = HAL_GetTick( );

            /*Wait for Auto-negotiation to complete*/
            do
            {
                HAL_ETH_ReadPHYRegister( &heth, PHY_BSR, &regVal );
                if( ( HAL_GetTick( ) - tickStart) > 1000 )
                {
                    /*Set Mac speed and Duplex mode to PHY*/
                    HAL_ETH_WritePHYRegister( &heth, PHY_BCR, 
                                            ( uint16_t )( heth.Init.DuplexMode >> 3 ) | 
                                            ( uint16_t )( heth.Init.Speed >> 1 ) );                       
                } 
                
            }while( ( regVal & PHY_AUTONEGO_COMPLETE ) != PHY_AUTONEGO_COMPLETE );

            /*Read the results of the auto-negotation*/
            if( ( regVal & PHY_DUPLEX_STATUS ) != ( uint32_t )( RESET ) )
            {
                /*Set ETH Duplex to full-duplex*/
                heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
            }
            else
            {
                /*Set ETH Duplex to half-duplex*/
                heth.Init.DuplexMode = ETH_MODE_HALFDUPLEX;
            }

            if( ( regVal & PHY_SPEED_STATUS )  )
            {
                /*Set ETH speed 10M*/
                heth.Init.Speed = ETH_SPEED_10M;
            }
            else
            {
                /*Set ETH Speed 100M*/
                heth.Init.Speed = ETH_SPEED_100M;
            }
        }
        else
        {
            /*Set MAC Speed and Duplex mode to PHY*/
            HAL_ETH_WritePHYRegister( &heth, PHY_BCR, 
                                    ( uint16_t )( heth.Init.DuplexMode >> 3 ) |
                                    ( uint16_t )( heth.Init.Speed >> 1 ) );
        }

        /*Re-configure ETH MAC*/
        HAL_ETH_ConfigMAC( &heth, ( ETH_MACInitTypeDef * )NULL );
        
        /*Restart MAC Interface*/
        HAL_ETH_Start( &heth );
    }
    else
    {
        /*Stop MAC*/
        HAL_ETH_Stop( &heth );
    }

    /*Notify changes*/
    notifyConnChanged( netif );

}

__weak void notifyConnChanged( struct netif *netif )
{
    //Do something, eg. Blink LED if link changed...
}








