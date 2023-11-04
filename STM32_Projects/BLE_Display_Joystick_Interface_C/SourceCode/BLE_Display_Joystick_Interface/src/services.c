/*
# ##############################################################################
# File: services.c                                                             #
# Project: src                                                                 #
# Created Date: Tuesday, October 24th 2023, 9:41:38 pm                         #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Wednesday, November 1st 2023, 9:18:32 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */

/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "bluenrg1_gap.h"
#include "bluenrg1_gatt_aci.h"
#include "services.h"
#include "main.h"
#include "i2c-lcd.h" 

/*##############################################################################################################################################*/
/*FUNCTION DECLARATIONS_________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/*128-bit UUID ( Randomly Generated )*/
const uint8_t SERVICE_UUID[ 16 ]    = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe0,0xf2,0x73,0xd9};
const uint8_t CHAR_UUID[ 16 ]       = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe1,0xf2,0x73,0xd9};
const uint8_t CHAR_UUID_RX[ 16 ]    = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe2,0xf2,0x73,0xd9};

uint16_t customServiceHdl, customCharacteristicHdl, chatRXCharacteristicHdl;

uint8_t     FLAG_CONNECTED              = FALSE;
uint8_t     FLAG_SET_CONNECTABLE        = TRUE;
uint8_t     FLAG_NOTIFICATION_ENABLED   = FALSE;

uint16_t    usrConnectionHdl            = 0;


uint8_t rcvData[ 20 ];

/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*EXTERNS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/**
 * @brief Add Service
 * 
 * @return tBleStatus 
 */
tBleStatus service_AddService( void )
{
    tBleStatus ret;
    
    Service_UUID_t  serviceUUID;
    Char_UUID_t     characteristicUUID, characteristicUUIDRX;
        
    memcpy( serviceUUID.Service_UUID_128, SERVICE_UUID , sizeof( SERVICE_UUID ) );
    memcpy( characteristicUUID.Char_UUID_128, CHAR_UUID, sizeof( CHAR_UUID ) );
    memcpy( characteristicUUIDRX.Char_UUID_128, CHAR_UUID_RX, sizeof( CHAR_UUID_RX ) );

    /*Add Service*/
    ret = aci_gatt_add_service(
        UUID_TYPE_128,
        &serviceUUID,
        PRIMARY_SERVICE,
        7,
        &customServiceHdl
    );

    /*Add Characteristic*/
    ret = aci_gatt_add_char( 
        customServiceHdl,
        UUID_TYPE_128,
        &characteristicUUID,
        15,
        CHAR_PROP_NOTIFY,
        ATTR_PERMISSION_NONE,
        GATT_DONT_NOTIFY_EVENTS,
        0,
        0,
        &customCharacteristicHdl
    );

    /*Add RX Characteristic*/
    ret = aci_gatt_add_char(
        customServiceHdl,
        UUID_TYPE_128,
        &characteristicUUIDRX,
        20,
        CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_WRITE,
        ATTR_PERMISSION_NONE,
        GATT_NOTIFY_ATTRIBUTE_WRITE,
        0,
        1,
        &chatRXCharacteristicHdl  
    );

    return ret;
}

/**
 * @brief Update Characteristic Value for Custom Service
 * 
 * @param newData 
 */
void service_UpdateData( int32_t newData )
{
    tBleStatus ret;
    newData = swap_int32( newData );

    /*Update Characteristics Value*/
    ret = aci_gatt_update_char_value(
        customServiceHdl,
        customCharacteristicHdl,
        0,
        4,
        ( uint8_t * )&newData
    );

    if( ret != BLE_STATUS_SUCCESS )
    {
        printf( " ACI GATT UPDATE CHARACTERISTIC VALUE: FAILED !! \r\n " );
    }
}

/*Converting from Big Endian to Little Endian*/
int32_t swap_int32(int32_t val) 
{
    return ((val << 24) & 0xFF000000) |
           ((val <<  8) & 0x00FF0000) |
           ((val >>  8) & 0x0000FF00) |
           ((val >> 24) & 0x000000FF);
}

/*Custom CallBack Implementations------------------------------------------------------------------------------*/

/**
 * @brief Custom Connection Complete CallBack. Used in GAP ( Generic Access Profile )
 * 
 * @param handle Connection Handle
 */
void GAP_customConnectionCompleteCB( uint16_t handle )
{
    FLAG_CONNECTED = TRUE;
    usrConnectionHdl = handle;
    printf( "CONNECTION COMPLETE ... \r\n" );
    HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET );
}

/**
 * @brief Custom Disconnection Complete CallBack. 
 * 
 */
void GAP_customDisconnectionCompleteCB( void )
{
    FLAG_CONNECTED = FALSE;
    usrConnectionHdl = 0;
    FLAG_SET_CONNECTABLE = TRUE;
    printf( " Disconnection Complete ... \r\n " );
    HAL_GPIO_WritePin( LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET );
}

void service_rcvData( uint8_t *dataBuff, uint8_t numBytes )
{
    HAL_GPIO_TogglePin( GPIOA, LED2_Pin );
    lcd_clear();
    lcd_put_cur( 0, 0 );
    memset( rcvData, 0, sizeof( rcvData ) );
    memcpy(rcvData, dataBuff, numBytes );
    lcd_send_string( ( char * )rcvData );
    HAL_Delay( 100 );
    printf( "MESSAGE RECIEVED: %s \r\n", rcvData );
}

void GATT_attributeModifiedCB( uint16_t handle, uint8_t dataLen, uint8_t *attributeData )
{
    if( handle == chatRXCharacteristicHdl + 1 ) //+ 1 to get Characteristic Value Declaration Handle
    {
        service_rcvData( attributeData, dataLen );
    }
}

/*Register Custom CallBacks---------------------------------------------------------------------------------*/

void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
{
    GATT_attributeModifiedCB( Attr_Handle, Attr_Data_Length, Attr_Data );
}

void hci_le_connection_complete_event
									(uint8_t Status,
									uint16_t Connection_Handle,
									uint8_t Role,
									uint8_t Peer_Address_Type,
									uint8_t Peer_Address[6],
									uint16_t Conn_Interval,
									uint16_t Conn_Latency,
									uint16_t Supervision_Timeout,
									uint8_t Master_Clock_Accuracy
									)
{
    GAP_customConnectionCompleteCB( Connection_Handle );
}

void hci_le_disconnection_complete_event
										(uint8_t Status,
										 uint16_t Connection_Handle,
										 uint8_t Reason
										)
{
    GAP_customDisconnectionCompleteCB( );
}


/*User Event Receive Function Implementation*/
void APP_userEvtRx( void * pData )
{
    uint32_t i;

    hci_spi_pckt *hciPckt = ( hci_spi_pckt * )pData;

    /*Process Event Packet*/
    if( hciPckt->type == HCI_EVENT_PKT )
    {
        /*Process Event Packet*/
        /*Get Data from Packet*/
        hci_event_pckt *eventPckt = ( hci_event_pckt * )hciPckt->data;

        if( eventPckt->evt == EVT_LE_META_EVENT )
        {
            
            /*Process Meta Data Event*/
            /*Get Meta Data*/
            evt_le_meta_event *metaDataEvt = ( void * )eventPckt->data;

            /*Loop through all the possible Meta events and process the specific event which occured*/
            for( i = 0; i < HCI_LE_META_EVENTS_COUNT; i++ )
            {
                if( metaDataEvt->subevent == hci_le_meta_events_table[ i ].evt_code )
                {
                    hci_le_meta_events_table[ i ].process( ( void * )metaDataEvt->data );
                }
            }
            
        } 
        else if( eventPckt->evt == EVT_VENDOR )
        {
            
            /*Process Vendor Event*/
            /*Get Vendor ( BlueNRG ) Data*/
            evt_blue_aci *blueNRGEvt = ( void * )eventPckt->data;

            /*Loop through all the possible Vendor Events and and process the specific event which occured*/
            for( i = 0; i < HCI_VENDOR_SPECIFIC_EVENTS_COUNT; i++ )
            {
                if( blueNRGEvt->ecode == hci_vendor_specific_events_table[ i ].evt_code )
                {
                    hci_vendor_specific_events_table[ i ].process( ( void * )blueNRGEvt->data );
                }
            }
            
        } 
        else
        {

            /*Process Normal Event*/
            /*Loop through all normal events and process the event which occured*/
            for( i = 0; i < HCI_EVENTS_COUNT; i++ )
            {
                if( eventPckt->evt == hci_events_table[ i ].evt_code )
                {
                    hci_events_table[ i ].process( ( void * )eventPckt->data );
                }
            }
            
        }
    }
    
    /*Process other events*/
    
}