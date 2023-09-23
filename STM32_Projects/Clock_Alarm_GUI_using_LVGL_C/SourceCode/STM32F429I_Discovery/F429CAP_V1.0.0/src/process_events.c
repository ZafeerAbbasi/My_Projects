/*
# ##############################################################################
# File: clock_process_events.c                                                 #
# Project: src                                                                 #
# Created Date: Sunday, September 17th 2023, 8:43:10 pm                        #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Friday, September 22nd 2023, 8:07:51 pm                       #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */


/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "process_events.h"

/*##############################################################################################################################################*/
/*FUNCTION DECLARATIONS_________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

static uint8_t getDayOfTheWeek(uint32_t year, uint32_t month, uint32_t day);

/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

const  char *g_days[] = { "Sunday", "Monday", "Tuesday", "Wednseday", "Thursday", "Friday", "Saturday" };

const char *g_months[] = { "January" , "February", "March","April","May", "June", "July", "August","September",\
                          "October","November","December" };


/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

void processEventEThemeSwitch( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Change State depending on switch value*/
    if( lv_obj_has_state( clkObject->gui_inst.themeSwitch, LV_STATE_CHECKED) )
    {
        /*Dark Mode*/
        clkObject->gui_inst.theme = THEME_DARK;
    }
    else
    {
        /*Light Mode*/
        clkObject->gui_inst.theme = THEME_LIGHT;
    }

    /*reload Main Page*/
    screenCleanup( &clkObject->gui_inst );
    guiCreateMainPageLabels( &clkObject->gui_inst );
    guiCreateMainPageStyle( &clkObject->gui_inst );
    guiUpdateCurrentDate( clkObject );
    guiUpdateCurrentTime( clkObject );

}

/**
 * @brief Process When a New Day has occured
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventENewDay( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*New day has occurred, increment day*/

    /*Set Time to 0*/
    clkObject->clock_inst.timeNow = 0;

    /*Increase Date by one*/
    clkObject->clock_inst.dateNow.date += 1;

    /*Change from PM to AM*/
    clkObject->clock_inst.timeMeridiemFormat = FORMAT_AM;

    /*Set day of the week*/
    clkObject->clock_inst.dateNow.day = getDayOfTheWeek( clkObject->clock_inst.dateNow.month, clkObject->clock_inst.dateNow.month, clkObject->clock_inst.dateNow.date );


    /*Reload based on current state*/
    if( clkObject->state == STATE_TICKING || clkObject->state == STATE_ALARM )
    {
        /*Update Time and Date*/
        guiUpdateCurrentDate( clkObject );
        guiUpdateCurrentTime( clkObject );
    }
    else if( clkObject->state == STATE_SETTING )
    {

        screenCleanup( &clkObject->gui_inst );
        /*Create Main Page*/
        //guiCreateMainPageLabels( &clkObject->gui_inst );
        processSettingsPage( clkObject, event );
    }
}

/**
 * @brief Process Event E_ALARM_NOTIF_CLOSE: Event generated when the user closes Alarm Notification page*
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEAlarmNotifClose( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Clean the Screen*/
    screenCleanup( &clkObject->gui_inst );

    if( clkObject->history == STATE_TICKING )
    {
        /*Previous state was main page*/

        /*Update State, clean screen and open main page*/
        GUI_STATE_CHANGE( clkObject, STATE_TICKING );
        screenCleanup( &clkObject->gui_inst );
        guiCreateMainPageLabels( &clkObject->gui_inst );
        guiCreateMainPageStyle( &clkObject->gui_inst );
        guiUpdateCurrentDate( clkObject );
        guiUpdateCurrentTime( clkObject );
    }
    else if( clkObject->history == STATE_SETTING )
    {
        /*Previous state was setting*/

        /*Load the setting Page*/
        processSettingsPage( clkObject, event );
    }
}

/**
 * @brief Process Event E_ALARM_NOTIF_ON: Event generated when the time hits the Alarm Time and the Alarm GIF Notif pops up
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEAlarmNotifON( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Save Current State and change it*/
    clkObject->history = clkObject->state;
    GUI_STATE_CHANGE( clkObject, STATE_ALARM );

    /*Clean screen and create Alarm Notification Page*/
    screenCleanup( &clkObject->gui_inst );
    guiCreateAlarmNotifPage( &clkObject->gui_inst );
}

/**
 * @brief Process Event E_SETTING_ALARM_ON_OFF: Event generated when user switches Alarm ON or OFF
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEAlarmONOFF( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiDateChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *alarmONOFFEvent = ( guiTimeChangeEvent_t *)event;

    /*Instantly Set Alarm Status*/
    clkObject->clock_inst.alarmStatus = alarmONOFFEvent->param;
}

/**
 * @brief Process Event E_SETTING_ALARM_SAVE: Event generated when user saves the Alarm Time in the setttings
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEAlarmSave( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Copy the user settings to settings save*/
    clkObject->setting_save.alarmMeridiemFormat = clkObject->usr_setting.alarmMeridiemFormat;
    clkObject->setting_save.alarmTime = clkObject->usr_setting.alarmTime;

    /*Get New time in 24H format*/
    uint32_t newTime = convert12HourFormatTo24Hour( clkObject->usr_setting.alarmTime, clkObject->usr_setting.alarmMeridiemFormat );

    /*Save the new time*/
    setAlarmTime( &clkObject->clock_inst, newTime );
}

/**
 * @brief Process Event E_SETTING_CLOCK_SAVE: Event generated when user saves Clock Time in the settings
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEClockSave( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Copy the user settings to settings save*/
    clkObject->setting_save.timeMeridiemFormat = clkObject->usr_setting.timeMeridiemFormat;
    clkObject->setting_save.timeNow = clkObject->usr_setting.timeNow;

    /*Save Time Meridiem*/
    clkObject->clock_inst.timeMeridiemFormat = clkObject->usr_setting.timeMeridiemFormat;

    /*Get New time in 24H format*/
    uint32_t newTime = convert12HourFormatTo24Hour( clkObject->usr_setting.timeNow, clkObject->usr_setting.timeMeridiemFormat );

    /*Save the new time*/
    setClockTime( &clkObject->clock_inst, newTime );
}

/**
 * @brief Process Event E_SETTING_DATE_SAVE: Event generated when user saves date on the calendar in settings
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEDateSave( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Copy the user settings to the settings save*/
    memcpy( &clkObject->setting_save.dateNow, &clkObject->usr_setting.dateNow, sizeof( date_t ) );

    /*Set the Main Clock Instance data as the settings save*/
    setClockDate( &clkObject->clock_inst, &clkObject->usr_setting.dateNow );
    guiUpdateCurrentDate( clkObject );
}

/**
 * @brief Process Event E_SETTING_DATE_CHANGE: Event generated when the user changes the date on the calendar
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEDateChange( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiDateChangeEvent_t, see respective EventHandler*/
    guiDateChangeEvent_t *dateChangeEvent = ( guiDateChangeEvent_t *)event;

    /*Set day of the week*/
    clkObject->usr_setting.dateNow.day = getDayOfTheWeek( dateChangeEvent->year, dateChangeEvent->month, dateChangeEvent->date );

    /*Set the rest of the date params*/
    clkObject->usr_setting.dateNow.date = dateChangeEvent->date;
    clkObject->usr_setting.dateNow.month = dateChangeEvent->month;
    clkObject->usr_setting.dateNow.year = dateChangeEvent->year;
}

/**
 * @brief Process Event E_SETTING_ALARM_MERIDIEM: Event generated when user changes Alarm format from 'AM' to 'PM' or vice versa
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEAlarmMeridiem( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeAlarmMeridiem = ( guiTimeChangeEvent_t *)event;

    /*Get Button ID*/
    uint8_t btnID = timeChangeAlarmMeridiem->param;   

    if( btnID == CHECK_BOX_INDEX_AM )
    {
        /*If User selected AM set accordingly*/
        clkObject->usr_setting.alarmMeridiemFormat = FORMAT_AM;
    }
    else if( btnID == CHECK_BOX_INDEX_PM )
    {
        /*If user selected PM, set accordingly*/
        clkObject->usr_setting.alarmMeridiemFormat = FORMAT_PM;
    }
}

/**
 * @brief Process Event E_SETTING_ALARM_MIN: Event generated when user rolls the roller to set a new value on the Alarm Minute roller
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEAlarmMin( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeAlarmMin = ( guiTimeChangeEvent_t *)event;

    /*Get new Min*/
    uint32_t newMin = timeChangeAlarmMin->param;

    /*Get Old Alarm Time*/
    uint32_t oldTime = clkObject->usr_setting.alarmTime;

    /*Get New Time*/
    uint32_t newTime = FORMAT_TIME( GET_HOUR( oldTime ), newMin, GET_SEC( oldTime ) );

    /*Update User Settings*/
    clkObject->usr_setting.alarmTime = newTime;
}

/**
 * @brief Process Event E_SETTING_ALARM_HOUR: Event generated when user rolls the roller to set a new value on the Alarm hour roller
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEAlarmHour( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeAlarmHour = ( guiTimeChangeEvent_t *)event;

    /*Get new Hour*/
    uint32_t newHour = timeChangeAlarmHour->param;

    /*Get Old Alarm Time*/
    uint32_t oldTime = clkObject->usr_setting.alarmTime;

    /*Get New Time*/
    uint32_t newTime = FORMAT_TIME( newHour, GET_MIN( oldTime ), GET_SEC( oldTime ) );

    /*Update User Settings*/
    clkObject->usr_setting.alarmTime = newTime;
}

/**
 * @brief Process Event E_SETTING_CLOCK_12H_24H: Event generated when user changes the Clock from '24 hour' to '12 hour' or vice versa
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEClock12H24H( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeClock12H24H = ( guiTimeChangeEvent_t *)event;

    /*Instantly Set the changes*/
    clkObject->clock_inst.time12H24H = timeChangeClock12H24H->param;
}

/**
 * @brief Process Event E_SETTING_CLOCK_MERIDIEM: Event generated when user changes the Clock 'AM' or 'PM' checkbox
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEClockMeridiem( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeClockMeridiem = ( guiTimeChangeEvent_t *)event;

    /*Get Button ID*/
    uint8_t btnID = timeChangeClockMeridiem->param;  

    if( btnID == CHECK_BOX_INDEX_AM )
    {
        /*If User selected AM set accordingly*/
        clkObject->usr_setting.timeMeridiemFormat = FORMAT_AM;
    }
    else if( btnID == CHECK_BOX_INDEX_PM )
    {
        /*If user selected PM, set accordingly*/
        clkObject->usr_setting.timeMeridiemFormat = FORMAT_PM;
    }
}

/**
 * @brief Process Event E_SETTING_CLOCK_SEC: Event generated when user rolls the roller to set a new value on the Time Second roller
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEClockSec( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeClockSec = ( guiTimeChangeEvent_t *)event;

    /*Get new Sec*/
    uint32_t newSec = timeChangeClockSec->param;

    /*Get Old Time*/
    uint32_t oldTime = clkObject->usr_setting.timeNow;

    /*Get New Time*/
    uint32_t newTime = FORMAT_TIME( GET_HOUR( oldTime ), GET_MIN( oldTime ), newSec );

    /*Update User Settings*/
    clkObject->usr_setting.timeNow = newTime;
}

/**
 * @brief Process Event E_SETTING_CLOCK_MIN: Event generated when user rolls the roller to set a new value on the Time Minute roller
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEClockMin( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeClockMin = ( guiTimeChangeEvent_t *)event;

    /*Get new Min*/
    uint32_t newMin = timeChangeClockMin->param;

    /*Get Old Time*/
    uint32_t oldTime = clkObject->usr_setting.timeNow;

    /*Get New Time*/
    uint32_t newTime = FORMAT_TIME( GET_HOUR( oldTime ), newMin, GET_SEC( oldTime ) );

    /*Update User Settings*/
    clkObject->usr_setting.timeNow = newTime;
}

/**
 * @brief Process Event E_SETTING_CLOCK_HOUR: Event generated when user rolls the roller to set a new value on the Time hour roller
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventEClockHour( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*Get the Full Time Change Event, since the passed into the function is just a member of guiTimeChangeEvent_t, see respective EventHandler*/
    guiTimeChangeEvent_t *timeChangeClockHour = ( guiTimeChangeEvent_t *)event;

    /*Get new Hour*/
    uint32_t newHour = timeChangeClockHour->param;

    /*Get Old Time*/
    uint32_t oldTime = clkObject->usr_setting.timeNow;

    /*Get New Time*/
    uint32_t newTime = FORMAT_TIME( newHour, GET_MIN( oldTime ), GET_SEC( oldTime ) );

    /*Update User Settings*/
    clkObject->usr_setting.timeNow = newTime;
}

/**
 * @brief Process Event E_SETTING_SAVE_YES_NO: Event generated when user selects either 'YES' or 'NO' at the 'Save Settings?' screen
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventESaveYesOrNo( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
    /*User has selected either 'Yes' or 'No' on the 'Save Settings?' messagebox*/

    /*Get the Full Event, since event is just a member of Full event*/
    guiGenericEvent_t *messageBoxEvent = (guiGenericEvent_t *)event;

    /*Placeholder variables*/
    uint32_t meridiem = 0;
    uint32_t newTime = 0;

    /*Check which button was pressed*/
    if( messageBoxEvent->param == SAVE_SETTING_YES )
    {
        /*User Wants to Save changes*/

        if( isDateSettingsChanged( clkObject ) )
        {
            /*If Date Settings were changed then update them*/
            setClockDate( &clkObject->clock_inst, &clkObject->usr_setting.dateNow );
        }

        if( isTimeSettingsChanged( clkObject ) )
        {
            /*If Time Settings were changed then update them*/
            meridiem = clkObject->usr_setting.timeMeridiemFormat;

            /*We always store the time in clock_inst as 24H but in usr_setting and setting_save we do 12H*/
            newTime = convert12HourFormatTo24Hour( clkObject->usr_setting.timeNow, meridiem );
            setClockTime( &clkObject->clock_inst, newTime );
        }

        if( isAlarmSettingsChanged( clkObject ) )
        {
            /*If Alarm Settings were Changed then update them*/
            meridiem = clkObject->usr_setting.alarmMeridiemFormat;

            /*We always store the time in clock_inst as 24H but in usr_setting and setting_save we do 12H*/
            newTime = convert12HourFormatTo24Hour( clkObject->usr_setting.alarmTime, meridiem );
            setAlarmTime( &clkObject->clock_inst, newTime );
        }
    }

    /*Update State, clean screen and open main page*/
    GUI_STATE_CHANGE( clkObject, STATE_TICKING );
    screenCleanup( &clkObject->gui_inst );
    guiCreateMainPageLabels( &clkObject->gui_inst );
    guiCreateMainPageStyle( &clkObject->gui_inst );

    /*Update Time and Date*/
    guiUpdateCurrentDate( clkObject );
    guiUpdateCurrentTime( clkObject );
}

/**
 * @brief Process Event E_SETTING_ROOT_BACK: Event generated when user clicks the '<' (Back) Btn on the settings screen
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventERootBack( ClockAlarmUI_t *const clkObject, guiEvent_t *event)
{
    /*User has pressed the 'Back' button on the settings page*/

    if( isSettingsChanged( clkObject ) )
    {
        /*Settings have changed*/
        static const char* options[] = { "Yes", "No", "" };

        /*Create Message Box for 'Save Changes?' */
        guiCreateMessageBox( &clkObject->gui_inst, "Settings", "Do you want to save changes?", options, false );
    }
    else
    {        
        /*Nothing has changed*/
        /*Update State*/
        GUI_STATE_CHANGE( clkObject, STATE_TICKING );

        /*Clean Screen*/
        screenCleanup( &clkObject->gui_inst );

        /*Create Main Page*/
        guiCreateMainPageLabels( &clkObject->gui_inst );

        /*Set Main Page style*/
        guiCreateMainPageStyle( &clkObject->gui_inst );

        /*Update Current time/date*/
        guiUpdateCurrentTime( clkObject );
        guiUpdateCurrentDate( clkObject );

    }
}

/**
 * @brief Process Event E_SETTING: Event generated when user clicks the 'Settings' option from the drop down list*
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processEventESetting( ClockAlarmUI_t *const clkObject, guiEvent_t *event)
{
    /*Extract event signal from event*/
    event_t currEvent = event->signal;

    /*Main Event Process switch statement*/
    if( currEvent == E_SETTING )
    {
        processSettingsPage( clkObject, event );
    }
}

/**
 * @brief Process the Settings Page ( Includes creating the setting GUI )
 * 
 * @param clkObject Main Clock Alarm Object
 * @param event Event 
 */
void processSettingsPage( ClockAlarmUI_t *const clkObject, guiEvent_t *event )
{
        /*CLOCK USER SETTINGS---------------------------------------------------------------------------------------------------*/
        /*Create a variable of type settingPageData_t to hold setting page data*/
        settingPageData_t settingPageData;
        memset(&settingPageData, 0, sizeof(settingPageData_t));

        /*Set user_setting->current time in 12 Hour format*/
        clkObject->usr_setting.timeNow = convert24HourFormatTo12Hour(clkObject->clock_inst.timeNow);
        clkObject->usr_setting.timeMeridiemFormat = clockGetAMorPM(clkObject->clock_inst.timeNow);
        clkObject->usr_setting.alarmTime = convert24HourFormatTo12Hour(clkObject->clock_inst.alarmTime);
        clkObject->usr_setting.alarmMeridiemFormat = clockGetAMorPM(clkObject->clock_inst.alarmTime);

        /*CLOCK SETTING SAVE----------------------------------------------------------------------------------------------------*/
        clkObject->setting_save.timeNow = clkObject->usr_setting.timeNow;
        clkObject->setting_save.timeMeridiemFormat = clkObject->usr_setting.timeMeridiemFormat;
        clkObject->setting_save.alarmTime = clkObject->usr_setting.alarmTime;
        clkObject->setting_save.alarmMeridiemFormat = clkObject->usr_setting.alarmMeridiemFormat;

        /*SETTING PAGE DATA - CLOCK---------------------------------------------------------------------------------------------*/
        /*Store current time in a temp variable*/
        uint32_t currentTime = clkObject->usr_setting.timeNow;
        settingPageData.clockHour = GET_HOUR(currentTime);
        settingPageData.clockMin = GET_MIN(currentTime);
        settingPageData.clockSecond = GET_SEC(currentTime);
        settingPageData.clockFormat = clockGetAMorPM(clkObject->clock_inst.timeNow);
        settingPageData.clock12H24H = clkObject->clock_inst.time12H24H;

        /*SETTING PAGE DATA - ALARM---------------------------------------------------------------------------------------------*/
        /*Store current Alarm time in temp variable*/
        currentTime = clkObject->usr_setting.alarmTime;
        settingPageData.alarmHour = GET_HOUR(currentTime);
        settingPageData.alarmMinute = GET_MIN(currentTime);
        settingPageData.alarmSecond = GET_SEC(currentTime);
        settingPageData.alarmFormat = clockGetAMorPM(clkObject->clock_inst.alarmTime);
        settingPageData.alarmStatus = clkObject->clock_inst.alarmStatus;
        
        /*DATE - SETTING PAGE DATA, USER SETTING, SETTING SAVE------------------------------------------------------------------*/
        /*Create a temporary variable to store current date*/
        date_t currentDate;
        
        /*Get current date and store into temp var currentDate*/
        clockGetDate(&clkObject->clock_inst, &currentDate);
        clkObject->usr_setting.dateNow = currentDate;
        clkObject->setting_save.dateNow = currentDate;

        /*Set Date in Setting Page data*/
        settingPageData.date = currentDate.date;
        settingPageData.day = currentDate.day;
        settingPageData.month = currentDate.month;
        settingPageData.year = currentDate.year;
        
        /*SCREEN UPDATES--------------------------------------------------------------------------------------------------------*/
        /*Clean Screen*/
        screenCleanup(&clkObject->gui_inst);

        /*Update State*/
        GUI_STATE_CHANGE( clkObject, STATE_SETTING );

        /*Create Settings Page*/
        guiCreateSettingsPage(&clkObject->gui_inst, &settingPageData);

        /*Update Current time/date*/
        guiUpdateCurrentTime( clkObject );
        guiUpdateCurrentDate( clkObject );
}

/**
 * @brief Update the Clock Display to Current Time
 * 
 * @param clkObject Main Clock Alarm UI Object
 */
void guiUpdateCurrentTime( ClockAlarmUI_t *const clkObject )
{
    char stringTime[10];
    /*Get the time in 24 Hours*/
    uint32_t time24H = clockGetCurrentTime( &clkObject->clock_inst );

    /*Get Time Mode*/
    uint32_t time12H24H = clockGetTime12H24H( &clkObject->clock_inst );

    /*Depending on mode get time in 12H or 24H*/
    uint32_t time;
    if( time12H24H == MODE_12H )
    {   
        time = convert24HourFormatTo12Hour( time24H);
        /*If we are in settings*/
        if( clkObject->state == STATE_SETTING || clkObject->state == STATE_ALARM )
        {
            convertTimeToString12Header( time, stringTime, clkObject->clock_inst.timeMeridiemFormat );
        }
        else
        {
            convertTimeToString12H( time, stringTime, clkObject->clock_inst.timeMeridiemFormat );
        }
    }
    else if( time12H24H == MODE_24H )
    {
        time = time24H;
        convertTimeToString( time, stringTime );
    }

    /*Convert time to string*/
    guiDisplayCurrentTime( &clkObject->gui_inst, stringTime );
}

/**
 * @brief Update Date depending on the current state ( TICKING or SETTING )
 * 
 * @param clkObject Main Clock Alarm UI Object
 */
void guiUpdateCurrentDate( ClockAlarmUI_t *const clkObject )
{
    /*Get Current Date*/
    date_t currentDate;
    clockGetDate( &clkObject->clock_inst, &currentDate );

    /*Update Date Depending on current state*/
    if( clkObject->state == STATE_SETTING )
    {
        guiDisplaySettingDateHeader( &clkObject->gui_inst, currentDate.date, currentDate.month, currentDate.year );
    }
    else if( clkObject->state == STATE_TICKING )
    {
        guiDisplayMainPageDay( &clkObject->gui_inst, g_days[ currentDate.day ] );
        guiDisplayMainPageMonth( &clkObject->gui_inst, g_months[ currentDate.month - 1 ] );
        guiDisplayMainPageDate( &clkObject->gui_inst, currentDate.date );
        guiDisplayMainPageYear( &clkObject->gui_inst, currentDate.year );
    }
}

/**
 * @brief Checks if Date Settings has changed
 * 
 * @param clkObject Main Clock Alarm Object
 * @return true if date has changed
 * @return false if date has not changed
 */
bool isDateSettingsChanged( ClockAlarmUI_t *const clkObject )
{
    /*Check if any of the date params is different than that of the stored user settings*/
    if( ( clkObject->setting_save.dateNow.day != clkObject->usr_setting.dateNow.day ) || \
    ( clkObject->setting_save.dateNow.date != clkObject->usr_setting.dateNow.date )   || \
    ( clkObject->setting_save.dateNow.month != clkObject->usr_setting.dateNow.month ) || \
    ( clkObject->setting_save.dateNow.year != clkObject->usr_setting.dateNow.year )      \
    )
    {
        return true;
    }
    else
    {
        /*Date has not changed*/
        return false;
    }
}

/**
 * @brief Checks if Time Settings has changed
 * 
 * @param clkObject Main Clock Alarm object
 * @return true if Time settings has changed
 * @return false if Time settings has not changed
 */
bool isTimeSettingsChanged( ClockAlarmUI_t *const clkObject)
{
    /*Get the current and setting save time in 24 Hours*/
    uint32_t usrTimeNow24H = convert12HourFormatTo24Hour( clkObject->usr_setting.timeNow, clkObject->usr_setting.timeMeridiemFormat );
    uint32_t savedTimeNow24H = convert12HourFormatTo24Hour( clkObject->setting_save.timeNow, clkObject->setting_save.timeMeridiemFormat );

    if( usrTimeNow24H != savedTimeNow24H )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Checks if Alarm Settings has changed
 * 
 * @param clkObject Main Clock Alarm Object
 * @return true if Alarm Settings has changed
 * @return false if Alarm Settings has not changed
 */
bool isAlarmSettingsChanged( ClockAlarmUI_t *const clkObject )
{
    /*Get the current and settings save Alarm time*/
    uint32_t usrAlarmTime24H = convert12HourFormatTo24Hour( clkObject->usr_setting.alarmTime, clkObject->usr_setting.alarmMeridiemFormat );
    uint32_t savedAlarmTime24H = convert12HourFormatTo24Hour( clkObject->setting_save.alarmTime, clkObject->setting_save.alarmMeridiemFormat );

    if( usrAlarmTime24H != savedAlarmTime24H )
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @brief Check if any of the settings has changed ( Date/Time/Alarm )
 * 
 * @param clkObject Main Clock Alarm UI Object
 * @return true If Any Settings has changed
 * @return false If no Settings has changed
 */
bool isSettingsChanged( ClockAlarmUI_t *const clkObject )
{
    bool retVal;

    /*Check each individual category if changed*/
    if( isDateSettingsChanged( clkObject ) )
    {
        retVal = true;
    }
    else if( isTimeSettingsChanged( clkObject ) )
    {
        retVal = true;
    }
    else if( isAlarmSettingsChanged( clkObject ) )
    {
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return retVal;
}

/**
 * @brief Converts Integer Time into string for header in settings
 * 
 * @param time Time represented in seconds
 * @param buffer Buffer to place string in, format: HH:MM:SS
 */
void convertTimeToString12Header( uint32_t time, char *const buffer, uint32_t meridiem)
{
    uint8_t hour, minute, second;
    char AMPM[3];
    hour = ( uint8_t )GET_HOUR( time );
    minute = ( uint8_t )GET_MIN( time );
    second = ( uint8_t )GET_SEC( time );

    if( meridiem )
    {
        strcpy( AMPM, "PM");
    }
    else
    {
        strcpy( AMPM, "AM");
    }

    sprintf( buffer, "%02d:%02d:%02d %s", hour, minute, second, AMPM);
}

/**
 * @brief Converts Integer Time into string 12H 
 * 
 * @param time Time represented in seconds
 * @param buffer Buffer to place string in, format: HH:MM:SS
 */
void convertTimeToString12H( uint32_t time, char *const buffer, uint32_t meridiem)
{
    uint8_t hour, minute, second;
    char AMPM[3];
    hour = ( uint8_t )GET_HOUR( time );
    minute = ( uint8_t )GET_MIN( time );
    second = ( uint8_t )GET_SEC( time );

    if( meridiem )
    {
        strcpy( AMPM, "PM");
    }
    else
    {
        strcpy( AMPM, "AM");
    }

    sprintf( buffer, "%02d:%02d:%02d\n%s", hour, minute, second, AMPM);
}

/**
 * @brief Converts Integer Time into string
 * 
 * @param time Time represented in seconds
 * @param buffer Buffer to place string in, format: HH:MM:SS
 */
void convertTimeToString( uint32_t time, char *const buffer)
{
    uint8_t hour, minute, second;
    hour = ( uint8_t )GET_HOUR( time );
    minute = ( uint8_t )GET_MIN( time );
    second = ( uint8_t )GET_SEC( time );

    sprintf( buffer, "%02d:%02d:%02d", hour, minute, second);
}

/**
 * @brief Convert 12 Hour Time to 24 Hour Time
 * 
 * @param time12H 12 Hour Time
 * @param meridiem Meridiem ( AM / PM )
 * @return uint32_t The time in 24 Hour
 */
uint32_t convert12HourFormatTo24Hour( uint32_t time12H, meridiemFormat_t meridiem )
{
    uint8_t currHour = GET_HOUR( time12H );
    uint32_t retValTime24H;

    if( meridiem == FORMAT_AM )
    {
        /*Check if its 12AM*/
        if( currHour == 12 )
        {
            /*If its 12 AM then subtract 12 from Hour to make the hour 0*/
            retValTime24H = ( time12H - FORMAT_TIME( 12, 0 , 0 ) );
        }
        else
        {
            /*In this case, 12H time is same as 24H time*/
            retValTime24H = time12H;
        }
    }
    else
    {
        /*Check if its 12PM*/
        if( currHour == 12 )
        {
            /*12PM is = 12 in 24H*/
            retValTime24H = time12H;
        }
        else
        {
            /*To conver 12H (PM) to 24H we add 12 hours*/
            retValTime24H = ( time12H + FORMAT_TIME( 12, 0, 0 ) );
        }
    }

    return retValTime24H;
}

/**
 * @brief Convert 24 Hour time format to 12 Hour time format
 * 
 * @param time24h 
 * @return uint32_t 
 */
uint32_t convert24HourFormatTo12Hour(uint32_t time24h) 
{
    
    /*Place holder variables*/
    uint8_t hour;
    uint32_t time12h;

    /*Get the hour*/
    hour = GET_HOUR(time24h);

    /*If hour == 0, then 0(24 Hour) = 12AM(12 Hour), so just place 12, in the hours section*/
    if (hour == 0)
        time12h = time24h + (12UL * 3600UL);
    else {

        /*If hour(24 hour) less than or equal to 12, then time(24 hour) == time(12 Hour)*/
        if ((hour < 12UL) || (hour == 12UL))
            return time24h;
        
        /*To convert 24 Hour time to 12 Hour, subtract 12 from hours*/
        else
            time12h = time24h - (12UL * 3600UL);
    }
    return time12h;
}

/**
 * @brief Get the Day Of The Week
 * 
 * @param year Year
 * @param month Month
 * @param day Date
 * @return uint8_t Day of the week
 */
static uint8_t getDayOfTheWeek(uint32_t year, uint32_t month, uint32_t day)
{
    uint32_t a = month < 3 ? 1 : 0;
    uint32_t b = year - a;

    uint32_t dayOfTheWeek = (day + (31 * (month - 2 + 12 * a) / 12) + b + (b / 4) - (b / 100) + (b / 400)) % 7;


    return dayOfTheWeek;
}

/*##############################################################################################################################################*/
/*EXTERNS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/