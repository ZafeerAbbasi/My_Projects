/*
# ##############################################################################
# File: clock_process_events.h                                                 #
# Project: include                                                             #
# Created Date: Sunday, September 17th 2023, 8:43:27 pm                        #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, September 17th 2023, 11:17:17 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */
#ifndef PROCESS_EVENTS
#define PROCESS_EVENTS



/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include "common_includes.h"


/*##############################################################################################################################################*/
/*FUNCTION DECLARATIONS_________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



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

void processEventEThemeSwitch( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventENewDay( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEAlarmNotifClose( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEAlarmNotifON( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEAlarmONOFF( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEAlarmSave( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEClockSave( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEDateSave( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEDateChange( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEAlarmMeridiem( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEAlarmMin( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEAlarmHour( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEClock12H24H( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEClockMeridiem( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEClockSec( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEClockMin( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventEClockHour( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventESaveYesOrNo( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
void processEventERootBack( ClockAlarmUI_t *const clkObject, guiEvent_t *event);
void processEventESetting( ClockAlarmUI_t *const clkObject, guiEvent_t *event);
void processSettingsPage( ClockAlarmUI_t *const clkObject, guiEvent_t *event );
uint32_t convert12HourFormatTo24Hour( uint32_t time12H, meridiemFormat_t meridiem );
uint32_t convert24HourFormatTo12Hour(uint32_t time24h);
void convertTimeToString( uint32_t time, char *const buffer);
void convertTimeToString12H( uint32_t time, char *const buffer, uint32_t meridiem);
void convertTimeToString12Header( uint32_t time, char *const buffer, uint32_t meridiem);
bool isSettingsChanged( ClockAlarmUI_t *const clkObject );
bool isAlarmSettingsChanged( ClockAlarmUI_t *const clkObject );
bool isTimeSettingsChanged( ClockAlarmUI_t *const clkObject);
bool isDateSettingsChanged( ClockAlarmUI_t *const clkObject );
void guiUpdateCurrentDate( ClockAlarmUI_t *const clkObject );
void guiUpdateCurrentTime( ClockAlarmUI_t *const clkObject );

/*##############################################################################################################################################*/
/*EXTERNS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/





#endif