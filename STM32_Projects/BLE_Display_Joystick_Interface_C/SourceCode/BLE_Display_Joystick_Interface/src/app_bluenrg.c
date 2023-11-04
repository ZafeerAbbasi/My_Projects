/*
# ##############################################################################
# File: app_bluenrg.c                                                          #
# Project: src                                                                 #
# Created Date: Wednesday, October 25th 2023, 5:29:08 pm                       #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Friday, October 27th 2023, 8:18:26 pm                         #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */
/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "app_bluenrg.h"
#include "services.h"
#include <stdio.h>

/*##############################################################################################################################################*/
/*FUNCTION DECLARATIONS_________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

uint8_t serverBTDeviceAddr[ ] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };

/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#define BADDR_SIZE      6

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
 * @brief Main BLUE NRG Init Function
 * 
 */
void blueNRG_init( void ) 
{
    const char *deviceName = "01_BLE";
    tBleStatus ret;
    uint8_t BTDeviceAddr[ BD_ADDR_SIZE ];
    memcpy( BTDeviceAddr, serverBTDeviceAddr, sizeof( serverBTDeviceAddr ) );
    uint16_t serviceHdl, devNameChracteristicHdl, appearanceCharacteristicHdl;

    /*1. Initialize HCI (Host Controller Interface) */
    /*2. Configure Device Address*/
    /*3. Initialize GATT ( Generic Attribute Profile ) Server*/
    /*4. Initialize GAP ( Generic Access Profile ) Service ( Mandatory Service )*/
    /*5. Update Characteristics*/
    /*6. Add Custom Service*/

    /*1. Initialize HCI ( Host Controller Interface ) */
    hci_init( APP_userEvtRx, NULL ); 
    hci_reset( );
    HAL_Delay( 2000 );

    /*2. Configure Device Address*/
    ret = aci_hal_write_config_data( CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, BTDeviceAddr );
    if( ret != BLE_STATUS_SUCCESS )
    {
        printf( " ACI HAL Write Config Data: FAILED !! \r\n " );
    }

    /*3. Initialize GATT Layer along with GATT Service and Server Changed Characteristic*/
    /*Background info:
    What Does the GATT Service Do?
    A GATT service is a collection of data and associated behaviors to accomplish a particular function or feature in a device. 
    It encapsulates one or more characteristics. Services enable a client to interact with the data on a GATT server in a structured 
    manner. A service might include such things as device information, custom data, or sensor readings.

    Adding the GATT Service with Service Changed Characteristic
    When the documentation says that it "also adds the GATT service with Service Changed Characteristic," it implies that as part of 
    this initialization process, a mandatory GATT Service is created and added to the device's GATT Server role. This GATT Service 
    is a collection of characteristics and behaviors that are standard across BLE devices.

    What Does the Service Changed Characteristic Do?
    The "Service Changed" characteristic is part of the mandatory GATT Service for a GATT Server. Its purpose is to inform connected 
    clients when any of the characteristics/services on the GATT Server have been changed, added, or removed. This allows the client to 
    rediscover the services and characteristics as needed.

    The "Service Changed" characteristic is particularly useful in situations where the server's available services and characteristics 
    can be dynamically modified. When the server updates, it can indicate this change to all connected clients, prompting them to refresh 
    their local cache of the server's attributes.

    So, in essence:

    "Service Changed" characteristic informs the GATT Client about changes in the services or characteristics on the GATT Server.
    This characteristic is part of the mandatory GATT Service on a GATT Server.*/
    ret = aci_gatt_init( );
    if( ret != BLE_STATUS_SUCCESS )
    {
        printf( " ACI GATT Init: FAILED !! \r\n " );
    }

    /*4. Initialize the GAP ( Generic Access Profile ) Layer. Also registers the Generic Access Service ( Mandatory Service )
    with the GATT ( Generic Attribute Profile ). All the standard GAP Characteristics are also added:
    - Device Name
    - Appearance
    - Peripheral Preferred Connnection Parameters ( Peripheral Role Only ) */
    ret = aci_gap_init( 
        GAP_PERIPHERAL_ROLE, 
        PRIVACY_DISABLED, 
        DEVICE_NAME_LEN,  
        &serviceHdl, 
        &devNameChracteristicHdl, 
        &appearanceCharacteristicHdl 
    );
    if( ret != BLE_STATUS_SUCCESS )
    {
        printf( " ACI GAP Init: FAILED !! \r\n " );
    }

    /*5. Update Device Name Characteristic Value */
    ret = aci_gatt_update_char_value( 
        serviceHdl, 
        devNameChracteristicHdl, 
        0, 
        strlen( deviceName ), 
        ( uint8_t * )deviceName
        );
    if( ret != BLE_STATUS_SUCCESS )
    {
        printf( " ACI GATT Characteristic Value Update: FAILED !! \r\n " );
    }

    /*6. Add Custom Service*/
    ret = service_AddService( );
    if( ret != BLE_STATUS_SUCCESS )
    {
        printf( " Add Simple Service: FAILED !! \r\n " );
    }
    
}

/**
 * @brief Main BLUE NRG Process Function
 * 
 */
void blueNRG_process( void )
{
    tBleStatus ret;
    uint8_t localName[ ] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'T', 'E', 'S', 'T', '-', 'T', 'R', 'Y' };
    
    ret = aci_gap_set_discoverable(
        ADV_IND, 
        0,
        0, 
        PUBLIC_ADDR,
        NO_WHITE_LIST_USE,
        sizeof( localName ),
        localName,
        0, 
        NULL,
        0, 
        0
    );

    hci_user_evt_proc( );
}
