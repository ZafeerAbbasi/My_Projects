/*
# ##############################################################################
# File: GUI.h                                                                  #
# Project: include                                                             #
# Created Date: Sunday, August 20th 2023, 10:45:20 pm                          #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, September 17th 2023, 11:07:03 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/
#ifndef GUI_H
#define GUI_H



/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "lvgl.h"


/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*DEFINES_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/



/*##############################################################################################################################################*/
/*TYPEDEFS/STRUCTS/ENUMS________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

/*Theme Enum*/
typedef enum
{
    THEME_LIGHT,
    THEME_DARK,
}theme_t;

/*Check Box Index Enum*/
typedef enum
{
    CHECK_BOX_INDEX_AM,
    CHECK_BOX_INDEX_PM
}checkBoxIndex;

/*Menu Item Builder Variant*/
typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
}menuItemBuilderVariant_t;

/*GUI Structure*/
typedef struct{
    uint32_t theme; 
    lv_color_t lightThemeColor;
    lv_color_t darkThemeColor;
    /*private*/
    lv_obj_t *screen;
    
    /*public*/
    lv_obj_t *clock;
    lv_obj_t *themeSwitch;

    /*private*/
    lv_obj_t *day;
    lv_obj_t *month;
    lv_obj_t *date;
    lv_obj_t *year;
    lv_obj_t *menu;
    lv_obj_t *cont;
    lv_obj_t *dropDownList;
    lv_obj_t *dropDownBtn;
    lv_style_t styleClock;
    lv_style_t styleBtnNormal;
    lv_style_t styleBtnClicked; 
    lv_style_t styleThemeChecked;        
}GUI_t;

/**
 * @brief Setting Page Data struct
 * 
 */
typedef struct{
    
    uint8_t clockHour;
    uint8_t clockMin;
    uint8_t clockSecond;
    uint8_t clockFormat;
    uint8_t clock12H24H;
    uint8_t alarmStatus;
    uint8_t alarmHour;
    uint8_t alarmMinute;
    uint8_t alarmSecond;
    uint8_t alarmFormat;
    uint8_t soundStatus;
    uint8_t brightness;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint32_t year;
}settingPageData_t;


/*##############################################################################################################################################*/
/*FUNCTIONS_____________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

void guiCreateAlarmNotifPage( GUI_t *const guiElement );
void guiCreateMessageBox(GUI_t *const guiElement, const char *title, const char *msg, const char *opts[], bool closeBtn );
void guiDisplayMainPageDay( GUI_t *const gui_element, const char *const day );
void guiDisplayMainPageMonth( GUI_t *const gui_element, const char *const month );
void guiDisplayMainPageDate( GUI_t *const gui_element, uint8_t date );
void guiDisplayMainPageYear( GUI_t *const gui_element, uint32_t year );
void guiDisplaySettingDateHeader( GUI_t *const gui_element, uint8_t date, uint8_t month , uint32_t year );
void guiDisplayCurrentTime( GUI_t *const guiElement, char *const stringTime );
void screenCleanup(GUI_t *gui_element);
void guiStyleCreator(GUI_t *const gui_element);
void lvglStyleInit(GUI_t *const gui_element);
void guiBtnStyleInit(GUI_t *const gui_element);
void guiCreateMainPageLabels(GUI_t *const gui_element);
void guiCreateMainPageStyle(GUI_t *const gui_element);
void guiCreateSettingsPage(GUI_t *const gui_element, settingPageData_t *settingpagedata);

#endif