/*
# ##############################################################################
# File: Project_ClockAlarm.c                                                   #
# Project: src                                                                 #
# Created Date: Monday, August 21st 2023, 2:09:43 am                           #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, September 17th 2023, 11:10:42 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/



/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include <stdio.h>
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
/*FUNCTIONS______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/**
 * @brief Main function to process all events 
 * 
 * @param clkObject Main Clock Alarm UI Object
 * @param event Pointer to the event which occurred
 */
void clockAlarmUIProcessEvent(ClockAlarmUI_t *const clkObject, guiEvent_t *event)
{
    /*Extract event signal from event*/
    event_t currEvent = event->signal;

    /*Main Event Process if-else statement*/
    if( currEvent == E_SETTING )
    {
        
        processEventESetting( clkObject, event );

    }
    else if( currEvent == E_SETTING_ROOT_BACK )
    {
        
        processEventERootBack( clkObject, event );

    }
    else if( currEvent == E_SETTING_SAVE_YES_NO )
    {
        
        processEventESaveYesOrNo( clkObject, event );

    }
    else if( currEvent == E_SETTING_CLOCK_HOUR )
    {
        
        processEventEClockHour( clkObject, event );

    }
    else if( currEvent == E_SETTING_CLOCK_MIN )
    {

        processEventEClockMin( clkObject, event );

    }
    else if( currEvent == E_SETTING_CLOCK_SEC )
    {

        processEventEClockSec( clkObject, event );
        
        
    }
    else if( currEvent == E_SETTING_CLOCK_MERIDIEM )
    {

        processEventEClockMeridiem( clkObject, event );

    }
    else if( currEvent == E_SETTING_CLOCK_12H_24H )
    {

        processEventEClock12H24H( clkObject, event );

    }
    else if( currEvent == E_SETTING_ALARM_HOUR )
    {

        processEventEAlarmHour( clkObject, event );

    }
    else if( currEvent == E_SETTING_ALARM_MIN )
    {

        processEventEAlarmMin( clkObject, event );

    }
    else if( currEvent == E_SETTING_ALARM_MERIDIEM )
    {

        processEventEAlarmMeridiem( clkObject, event );

    }
    else if( currEvent == E_SETTING_DATE_CHANGE )
    {

        processEventEDateChange( clkObject, event );

    }
    else if( currEvent == E_SETTING_DATE_SAVE )
    {
        
        processEventEDateSave( clkObject, event );

    }
    else if( currEvent == E_SETTING_CLOCK_SAVE )
    {

        processEventEClockSave( clkObject, event );

    }
    else if( currEvent == E_SETTING_ALARM_SAVE )
    {

        processEventEAlarmSave( clkObject, event );

    }
    else if( currEvent == E_SETTING_ALARM_ON_OFF )
    {

        processEventEAlarmONOFF( clkObject, event );

    }
    else if( currEvent == E_ALARM_NOTIF_ON )
    {

        processEventEAlarmNotifON( clkObject, event );

    }
    else if( currEvent == E_ALARM_NOTIF_CLOSE )
    {

        processEventEAlarmNotifClose( clkObject, event );

    }
    else if( currEvent == E_NEW_DAY )
    {

        processEventENewDay( clkObject, event );
        
    }
    else if( currEvent == E_THEME_SWITCH )
    {

        processEventEThemeSwitch( clkObject, event );

    }
}

/**
 * @brief Main Clock Alarm UI Object Constructor
 * 
 * @param clkObject Main Clock Alarm UI Object
 */
void clockAlarmUIConstructor(ClockAlarmUI_t *const clkObject)
{
    /*Update State to STATE_TICKING*/
    GUI_STATE_CHANGE(clkObject, STATE_TICKING);

    /*Create a Clock (i.e. Initalize clock_t element of main clock object)*/
    createClock(&clkObject->clock_inst);

    /*Create a GUI (i.e. Initialize the GUI button styles)*/
    guiStyleCreator(&clkObject->gui_inst);

    /*Assign label objects to each element of gui_inst of main clock object and callbacks to screen and main dropDownBtn*/
    guiCreateMainPageLabels(&clkObject->gui_inst);

    /*Initialize style for main page*/
    guiCreateMainPageStyle(&clkObject->gui_inst);

    /*Update Current Time and Date*/
    guiUpdateCurrentTime( clkObject );
    guiUpdateCurrentDate( clkObject );
}

