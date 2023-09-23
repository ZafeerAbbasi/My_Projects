/*
# ##############################################################################
# File: gui_widget.h                                                           #
# Project: include                                                             #
# Created Date: Monday, August 21st 2023, 4:49:16 am                           #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, September 17th 2023, 11:19:25 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/
#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H


/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "gui.h"

/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/**
 * @brief Radio Btn Struct
 * 
 */
typedef struct
{
    uint8_t     CheckBoxSettingType;
    uint32_t    CheckBoxBoxIndex;
}CheckBoxData_t;

/**
 * @brief Roller Settings Enum
 * 
 */
typedef enum
{
    SETTING_TIME,
    SETTING_ALARM
}settingType_t;

/**
 * @brief Roller Type Enum
 * 
 */
typedef enum
{
    ROLLER_HOUR,
    ROLLER_MIN, 
    ROLLER_SEC
}rollerType_t;

/**
 * @brief Roller Data Struct
 * 
 */
typedef struct
{
    uint8_t     rollerType;
    uint8_t     settingType;
    int         rollerActiveVal;
}rollerData_t;

/**
 * @brief Calendar Data Struct
 * 
 */
typedef struct
{
    uint32_t    year;
    uint32_t    month;
    uint32_t    day;
    uint8_t     date;
}calendarData_t;


/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

void eventHandlerThemeSwitch( lv_event_t *event );
void eventHandlerNotifClose( lv_event_t *event );
void eventHandlerMessageBox( lv_event_t *event );
void eventHandlerDisplaySlider( lv_event_t *event );
void eventHandlerAlarmONOFFSwitch( lv_event_t *event );
void eventHandlerAlarmRollers( lv_event_t *event );
void eventHandlerTimeSave( lv_event_t *event );
void eventHandler12H24HSwitch( lv_event_t *event );
void eventHandlerMeridiemCheckBoxes( lv_event_t *event );
void eventHandlerTimeRollers( lv_event_t *event );
void eventHandlerDateSave( lv_event_t *event );
void eventHandlerDateChange( lv_event_t *event );
void eventHandlerRootBackBtn( lv_event_t *event );
void eventHandlerDropDownBtn( lv_event_t *e );
void eventHandlerScreen( lv_event_t * e );
void eventHandlerDropDownBtnOptionSettings( lv_event_t *e );
void eventHandlerDropDownBtnOptionAbout( lv_event_t* e );
void collapseDropDownList( GUI_t *gui_element );
lv_obj_t *createMenu( lv_obj_t *parent, bool enableRootBtn, lv_event_cb_t eventCallBack );
lv_obj_t *createCalendar( lv_obj_t *parent, calendarData_t *data );
lv_obj_t *createSaveBtn( lv_obj_t *parent, const char* txt, lv_style_t *normal, lv_style_t *clicked, lv_event_cb_t eventCallBack );
lv_obj_t *createRoller( lv_obj_t *parent, const char *opts, int currVal );
lv_obj_t *createCheckBox( lv_obj_t *parent, const char *txt, lv_style_t *styleCheckBox, lv_style_t *styleCheckBoxSelected );
lv_obj_t *createONOFFSwitch( lv_obj_t *parent, const char *icon, const char *txt, bool currSwitchVal, lv_event_cb_t eventCallBack );
lv_obj_t *createLabelContainer( lv_obj_t *parent, const char *icon, const char *txt, menuItemBuilderVariant_t builderVariant );
lv_obj_t *createDisplaySlider( lv_obj_t *parent, const char *icon, const char*txt, int32_t min, int32_t max, int32_t currVal, lv_event_cb_t eventCallBack );
lv_obj_t *createMessageBox( lv_obj_t *parent, const char *title, const char *msg, const char*opts[], bool closeBtn );

extern ClockAlarmUI_t clockAlarmUI_inst;

#endif