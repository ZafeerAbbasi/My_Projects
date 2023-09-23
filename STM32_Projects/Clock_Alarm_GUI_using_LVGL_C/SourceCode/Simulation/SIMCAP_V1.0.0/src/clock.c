/*
# ##############################################################################
# File: clock.c                                                                #
# Project: src                                                                 #
# Created Date: Monday, August 21st 2023, 3:03:30 am                           #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, September 17th 2023, 10:39:39 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/
/*This file is about manipulating data regarding the clock_t element of the Main Clock object*/


/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "clock.h"
#include "common_includes.h"
#include "project_clock_alarm.h"



/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/**
 * @brief Checks if Current time == Alarm Time
 * 
 * @param clkData Clock Data
 * @return true If Current Time == Alarm Time
 * @return false If Current Time != Alarm Time
 */
bool isAlarm( clock_t *const clkData )
{
    return ( clkData->timeNow == clkData->alarmTime );
}

/**
 * @brief Update Current Time in the DataBase
 * 
 * @param clkData 
 */
void dataBaseUpdateCurrentTime( clock_t *const clkData )
{
    /*Increment Time*/
    clkData->timeNow += 1;

    /*Check if max time reached (End of day)*/
    if( clkData->timeNow >= 86400UL )
    {
        /*Reset Time*/
        clkData->timeNow = 0;

        /*Create Event for Newday and process it*/
        guiDateChangeEvent_t newDay;
        newDay.mainEvent.signal = E_NEW_DAY;
        clockAlarmUIProcessEvent( &clockAlarmUI_inst, &newDay.mainEvent );
    }
}

/**
 * @brief Set the Alarm Time
 * 
 * @param clkData Clock Data to store the new Alarm Time
 * @param newTime Source of the Alarm Time
 */
void setAlarmTime( clock_t *const clkData, uint32_t newTime )
{
    clkData->alarmTime = newTime;
}

/**
 * @brief Set the Clock Time 
 * 
 * @param clkData Clock Data to store the new Time
 * @param newTime Source of Time Data
 */
void setClockTime( clock_t *const clkData, uint32_t newTime )
{
    clkData->timeNow = newTime;
}

/**
 * @brief Set the Clock Date 
 * 
 * @param clkData Clock Data to store the new date
 * @param newDate Source of Date Data
 */
void setClockDate( clock_t *const clkData, date_t *const newDate )
{
    /*Set ClkData = NewDate*/
    clkData->dateNow.date = newDate->date;
    clkData->dateNow.day = newDate->day;
    clkData->dateNow.month = newDate->month;
    clkData->dateNow.year = newDate->year;
}

/**
 * @brief Get the current Clock Time
 * 
 * @param clk_inst Source of Date info
 * @return uint32_t Value of current time in 24 hours, units: seconds
 */
uint32_t clockGetCurrentTime( clock_t *const clkData )
{
    return clkData->timeNow;
}

/**
 * @brief Get the current Clock Mode
 * 
 * @param clkData Source of Data info
 * @return uint8_t Current Clock Mode (24H/12H)
 */
uint8_t clockGetTime12H24H( clock_t *const clkData )
{
    return clkData->time12H24H;
}

/**
 * @brief Gets date info from clk inst and adds it to second arg
 * 
 * @param clk_inst Source of date info
 * @param date Destination of date info
 */
void clockGetDate(clock_t *const clk_inst, date_t *const date)
{
    date->date = clk_inst->dateNow.date;
    date->day = clk_inst->dateNow.day;
    date->month = clk_inst->dateNow.month;
    date->year = clk_inst->dateNow.year;
}

/**
 * @brief Take in a clock instance of type clock_t and set initial date/time
 * 
 * @param clock_element clock_t member of the main ClockAlarmUI_t object, i.e. clock_inst 
 */
void createClock(clock_t *const clock_element)
{
    clock_element->timeNow              = INITIAL_CURRENT_TIME;
    clock_element->time12H24H             = MODE_12H;
    clock_element->alarmTime            = INITIAL_ALARM_TIME;
    clock_element->dateNow.day          = INITIAL_DAY;
    clock_element->dateNow.date         = INITIAL_DATE;
    clock_element->dateNow.month        = INITIAL_MONTH;
    clock_element->dateNow.year         = INITIAL_YEAR;
    clock_element->timeMeridiemFormat   = FORMAT_AM;
}

/**
 * @brief Take in 24Hour time and return Am or PM
 * 
 * @param time24h 
 * @return uint8_t 
 */
uint8_t clockGetAMorPM(uint32_t time24h)
{
    uint8_t meridiem;
    uint8_t hour = (uint8_t)GET_HOUR(time24h);
    if (hour == 0U) {
        meridiem = FORMAT_AM;
    }
    else if (hour > 12U) {
        meridiem = FORMAT_PM;
    }
    else if (hour == 12U)
        meridiem = FORMAT_PM;
    else
        meridiem = FORMAT_AM;
    return meridiem;
}