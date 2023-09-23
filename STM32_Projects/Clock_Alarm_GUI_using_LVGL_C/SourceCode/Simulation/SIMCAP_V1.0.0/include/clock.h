/*
# ##############################################################################
# File: Clock.h                                                                #
# Project: include                                                             #
# Created Date: Sunday, August 20th 2023, 9:51:28 pm                           #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Friday, September 1st 2023, 8:13:28 am                        #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/
#ifndef CLOCK_H
#define CLOCK_H



/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include <stdint.h>
#include <stdbool.h>



/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#define THURSDAY 4
#define AUGUST  7
#define MAX_TIME  (86400UL)
#define INITIAL_CURRENT_TIME ( ( 0 * 3600 ) + ( 12 * 60 ) + ( 12 ) )
#define INITIAL_ALARM_TIME  ( 8 * 3600 )
#define INITIAL_DAY THURSDAY
#define INITIAL_MONTH  AUGUST
#define INITIAL_YEAR  2023UL
#define INITIAL_DATE  12
#define INITIAL_alarmStatus ALARM_ON  
#define BASE_YEAR 2000

/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/*Alarm ON/OFF Enum*/
typedef enum
{
    ALARM_ON,
    ALARM_OFF,
}alarmONOFF_t;

/*Date structure*/
typedef struct
{
    uint32_t day;
    uint32_t date;
    uint32_t month;
    uint32_t year;
}date_t;

/*Clock Structure*/
typedef struct
{
    uint32_t    timeNow;
    uint32_t    alarmTime;
    uint32_t    timeMeridiemFormat;
    uint8_t     alarmMeridiemFormat;
    uint32_t    alarmStatus;
    uint32_t    time12H24H;
    date_t      dateNow;
}clock_t;

/*Time Mode (24HR/12HR) Enum*/
typedef enum{

    MODE_24H,
    MODE_12H
}time12H24H_t;

/*Time Format ( AM / PM / 24HR ) Enum*/
typedef enum
{
    FORMAT_AM,
    FORMAT_PM
}meridiemFormat_t;

/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

bool isAlarm( clock_t *const clkData );
void dataBaseUpdateCurrentTime( clock_t *const clkData );
void setAlarmTime( clock_t *const clkData, uint32_t newTime );
void setClockTime( clock_t *const clkData, uint32_t newTime );
void setClockDate( clock_t *const clkData, date_t *const newDate );
void createClock(clock_t *const clock_element);
uint8_t clockGetAMorPM(uint32_t time24h);
void clockGetDate(clock_t *const clk_inst, date_t *const date);
uint32_t clockGetCurrentTime( clock_t *const clkData );
uint8_t clockGetTime12H24H( clock_t *const clkData );


#endif