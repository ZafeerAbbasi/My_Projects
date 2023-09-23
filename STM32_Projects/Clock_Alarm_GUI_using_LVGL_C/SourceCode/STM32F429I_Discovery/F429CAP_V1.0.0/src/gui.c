/*
# ##############################################################################
# File: gui.c                                                                  #
# Project: src                                                                 #
# Created Date: Monday, August 21st 2023, 4:46:40 am                           #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, September 17th 2023, 11:15:28 pm                      #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/
/*This File is about all the GUI Implemnetation excluding widgets*/



/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "gui.h"
#include "lvgl.h"
#include "project_clock_alarm.h"
#include "clock.h"
#include "gui_widget.h"
#include <stdio.h>
#include "examples/lv_examples.h"

/*##############################################################################################################################################*/
/*GLOBALS_______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

static void guiCreateTimeDateHeader(GUI_t *const gui_element);
static lv_obj_t *createSettingsTimeAlarmRollersRow(lv_obj_t *parent, rollerData_t *rollerHour, rollerData_t *rollerMin, rollerData_t *rollerSec, lv_event_cb_t eventCallBack);
static void createMeridiemCheckBoxes(lv_obj_t *parent, CheckBoxData_t *CheckBoxData, GUI_t *const guiElement );

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
 * @brief Create the Alarm Notification Page
 * 
 * @param guiElement GUI Element of the main clock object
 */
void guiCreateAlarmNotifPage( GUI_t *const guiElement )
{
    /*Create Time Date Header*/
    guiCreateTimeDateHeader( guiElement );

    /*Declare New Image ( Bell Notif GIF )*/
    LV_IMG_DECLARE( bellNotifGIF );

    /*Create GIF*/
    lv_obj_t *img = lv_gif_create( guiElement->screen );
    lv_gif_set_src( img, &bellNotifGIF );
    lv_obj_align( img, LV_ALIGN_CENTER, 0, 0 );

    /*Create Close Button*/
    lv_obj_t *closeBtn = createSaveBtn( guiElement->screen, "Close", &guiElement->styleBtnNormal, &guiElement->styleBtnClicked, eventHandlerNotifClose );

    /*Align the Close Button*/
    lv_obj_align_to( closeBtn, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );
}

/**
 * @brief 
 * 
 * @param guiElement GUI element of main Clock Alarm object
 * @param title the title of the message box
 * @param msg the text of the message box
 * @param opts the buttons as an array of texts terminated by an "" element. E.g. {"btn1", "btn2", ""}
 * @param closeBtn add a close button
 */
void guiCreateMessageBox(GUI_t *const guiElement, const char *title, const char *msg, const char *opts[], bool closeBtn )
{
    lv_obj_t *messageBox = createMessageBox( NULL, title, msg, opts, closeBtn );

    if( guiElement->theme == THEME_DARK )
    {
        lv_obj_set_style_border_color( messageBox, lv_color_white(), LV_PART_MAIN );
        lv_obj_set_style_text_color( messageBox, lv_color_white(), LV_PART_MAIN );
        lv_obj_set_style_bg_color( messageBox, lv_color_make( 13, 17, 23 ), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_color( messageBox, lv_color_make( 40, 52, 71 ), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( messageBox, LV_GRAD_DIR_VER, LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_border_color( messageBox, lv_color_black(), LV_PART_MAIN );
        lv_obj_set_style_text_color( messageBox, lv_color_black(), LV_PART_MAIN );
        lv_obj_set_style_bg_color( messageBox, lv_color_make(255, 211, 165), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_color( messageBox, lv_color_make(213, 145, 142), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( messageBox, LV_GRAD_DIR_VER, LV_PART_MAIN );
    }
}

/**
 * @brief Display the Day on the Main Clock Page
 * 
 * @param gui_element GUI Element of the Main Clock Object
 * @param date Source of Date information
 */
void guiDisplayMainPageDay( GUI_t *const gui_element, const char *const day )
{
    lv_label_set_text( gui_element->day, day );
}

/**
 * @brief Display the Month on the Main Clock Page
 * 
 * @param gui_element GUI Element of the Main Clock Object
 * @param date Source of Date information
 */
void guiDisplayMainPageMonth( GUI_t *const gui_element, const char *const month )
{
    lv_label_set_text( gui_element->month, month );
}

/**
 * @brief Display the Date on the Main Clock Page
 * 
 * @param gui_element GUI Element of the Main Clock Object
 * @param date Source of Date information
 */
void guiDisplayMainPageDate( GUI_t *const gui_element, uint8_t date )
{
    lv_label_set_text_fmt( gui_element->date, "%d", date );
}

/**
 * @brief Display the Year on the Main Clock Page
 * 
 * @param gui_element GUI Element of the Main Clock Object
 * @param date Source of Date information
 */
void guiDisplayMainPageYear( GUI_t *const gui_element, uint32_t year )
{
    lv_label_set_text_fmt( gui_element->year,"%ld", year );
}

/**
 * @brief Display the Date Header on Settings Page
 * 
 * @param gui_element GUI Element of the Main Clock Object
 * @param date Source of Date information
 */
void guiDisplaySettingDateHeader( GUI_t *const gui_element, uint8_t date, uint8_t month , uint32_t year )
{
    lv_label_set_text_fmt( gui_element->date, "%d/%d/%ld", date, month, year );
}

/**
 * @brief Update the GUI Clock Time Label
 * 
 * @param guiElement GUI Element of the Main Clock Object
 * @param stringTime String time in format HH:MM:SS
 */
void guiDisplayCurrentTime( GUI_t *const guiElement, char *const stringTime )
{
    /*Update Clock Time Label*/
    lv_label_set_text( guiElement->clock, stringTime );
}

/**
 * @brief Create a Time Format Check Boxes object
 * 
 * @param parent 
 * @param CheckBoxData 
 */
static void createMeridiemCheckBoxes(lv_obj_t *parent, CheckBoxData_t *CheckBoxData, GUI_t *const guiElement )
{
    /*Create container on parent param passed into this function*/
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_style_bg_opa( container, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Set Flex Flow of container to coloumn to add buttons in vertical order*/
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);

    if( guiElement->theme == THEME_DARK )
    {
        lv_obj_set_style_border_color( container, lv_color_white(), LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_border_color( container, lv_color_black(), LV_PART_MAIN );
    }

    /*Set height and width of container*/
    lv_obj_set_width(container, LV_SIZE_CONTENT);
    lv_obj_set_height(container, LV_SIZE_CONTENT);

    /*Add event callback*/
    lv_obj_add_event_cb(container, eventHandlerMeridiemCheckBoxes, LV_EVENT_CLICKED, CheckBoxData);

    /*Create CheckBoxes*/
    createCheckBox(container, "AM", NULL, NULL); /*Created first, thus ID for this Btn = 0*/
    createCheckBox(container, "PM", NULL, NULL); /*Created second, thus ID for this Btn = 1*/

    /*Set Checked State for the Button depending on Time format */
    lv_obj_add_state(lv_obj_get_child(container, CheckBoxData->CheckBoxBoxIndex), LV_STATE_CHECKED);

}

/**
 * @brief Create a Row of Rollers for Settings Page Time and Alarm
 * 
 * @param parent Parent to which to place the Roller objects on
 * @param rollerHour Pointer to Hour rollerData_t struct
 * @param rollerMin Pointer to Minute rollerData_t struct
 * @param rollerSec Pointer to Second rollerData_t struct
 * @param eventCallBack eventCallBack Roller Event Callback
 * @return lv_obj_t* Pointer to Parent Row object
 */
static lv_obj_t *createSettingsTimeAlarmRollersRow(lv_obj_t *parent, rollerData_t *rollerHour, rollerData_t *rollerMin, rollerData_t *rollerSec, lv_event_cb_t eventCallBack)
{
    /*Set texts containing roller selections*/
    const char* optsHour = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12";

    const char* optsMinSec = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12 \
                          \n13\n14\n15\n16\n17\n18\n19\n20\n21\n22      \
                          \n23\n24\n25\n26\n27\n28\n29\n30\n31\n32      \
                          \n33\n34\n35\n36\n37\n38\n39\n40\n41\n42      \
                          \n43\n44\n45\n46\n47\n48\n49\n50\n51\n52      \
                          \n53\n54\n55\n56\n57\n58\n59";

    
    /*Create roller objects for Hour, Minute, and Second*/
    lv_obj_t *rollerHourObject;
    lv_obj_t *rollerMinObject;
    lv_obj_t *rollerSecObject;

    /*Create a Parent object to place the roller containers on*/
    lv_obj_t *containerRow = lv_obj_create(parent);

    /*Set size, set width to LV_SIZE_CONTENT so that LVGL will dynamically adjust the width of the
    containerRow object when we add child objects to it, and set height to 100*/
    lv_obj_set_size(containerRow, LV_SIZE_CONTENT, 100);

    /*Set Flex Flow, the flex flow function is used to set the flow direction of children inside a
    container. So now child items will be laid horizontally instead of vertically*/
    lv_obj_set_flex_flow(containerRow, LV_FLEX_FLOW_ROW);

    /*Create label for hour roller*/
    lv_obj_t *labelHour = lv_label_create(containerRow);
    lv_label_set_text(labelHour, "hh");

    /*Create Hour roller*/
    rollerHourObject = createRoller(containerRow, optsHour, rollerHour->rollerActiveVal);
    lv_obj_set_style_bg_opa( rollerHourObject, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Set Roller type for the Roller Hour Struct passed into the function*/
    rollerHour->rollerType = ROLLER_HOUR;

    /*Add Callback for the rollerHourObject that we created in this function*/
    lv_obj_add_event_cb(rollerHourObject, eventCallBack, LV_EVENT_VALUE_CHANGED, rollerHour);

    /*Create Label for Minute Roller*/
    lv_obj_t *labelMin = lv_label_create(containerRow);
    lv_label_set_text(labelMin, "mm");

    /*Create Minute Roller*/
    rollerMinObject = createRoller(containerRow, optsMinSec, rollerMin->rollerActiveVal);
    lv_obj_set_style_bg_opa( rollerMinObject, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Set Roller type for the Roller Min struct passed into this function*/
    rollerMin->rollerType = ROLLER_MIN;

    /*Add Callback for the RollerMin object created in this function*/
    lv_obj_add_event_cb(rollerMinObject, eventCallBack, LV_EVENT_VALUE_CHANGED, rollerMin);

    if ( rollerSec != NULL )
    {
        /*If rollerSec exits*/
        /*Create Lable for Second Roller*/
        lv_obj_t *labelSec = lv_label_create(containerRow);
        lv_label_set_text(labelSec, "ss");

        /*Create Second roller*/
        rollerSecObject = createRoller(containerRow, optsMinSec, rollerSec->rollerActiveVal);
        lv_obj_set_style_bg_opa( rollerSecObject, LV_OPA_TRANSP, LV_PART_MAIN );

        /*Set Roller type for the roller sec struct passed into this function*/
        rollerSec->rollerType = ROLLER_SEC;

        /*Add callback for the RolleSec object created in this function*/
        lv_obj_add_event_cb(rollerSecObject, eventCallBack, LV_EVENT_VALUE_CHANGED, rollerSec);
    }
    return containerRow;
}

/**
 * @brief Create Time and Date Header for settings page
 * 
 * @param gui_element 
 */
static void guiCreateTimeDateHeader(GUI_t *const gui_element)
{
    /*Since gui_inst is cleaned we can assign the headers to the clock and date*/
    /*CLOCK-------------------------------------------------------------------------------------------------------------------------------------*/
    gui_element->clock = lv_label_create(gui_element->screen);
    
    /*Assign blank text at first, will update later*/
    lv_label_set_text(gui_element->clock, "");
    
    /*Set Font*/
    lv_obj_set_style_text_font(gui_element->clock, &lv_font_montserrat_14, 0);
    
    /*Align Clock to Top right*/
    lv_obj_align(gui_element->clock, LV_ALIGN_TOP_RIGHT, -5, 0);

    /*DATE--------------------------------------------------------------------------------------------------------------------------------------*/
    gui_element->date = lv_label_create(gui_element->screen);
    
    /*Assign blank text at first, will update later*/
    lv_label_set_text(gui_element->date, "");

    /*Set font*/
    lv_obj_set_style_text_font(gui_element->date, &lv_font_montserrat_14, 0);

    /*Align date to top left*/
    lv_obj_align(gui_element->date, LV_ALIGN_TOP_LEFT, 5, 0);

    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_set_style_text_color(gui_element->clock, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_color(gui_element->date, lv_color_white(), LV_PART_MAIN);
    }
}

/**
 * @brief Create Settings Page
 * 
 * @param gui_element Gui Instance
 * @param settingpagedata Setting Page data
 */
void guiCreateSettingsPage(GUI_t *const gui_element, settingPageData_t *settingpagedata)
{
    /*Placeholder variables*/
    lv_obj_t* rootPage;
    lv_obj_t* container;
    lv_obj_t* defaultcontainerToShow;
    lv_obj_t* section;
    lv_obj_t* parentRow;
    lv_obj_t* menu;

    /*Create Time and Date Header*/
    guiCreateTimeDateHeader(gui_element);

    /*Create main settings menu*/
    menu = createMenu(gui_element->screen, true, eventHandlerRootBackBtn);

    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_set_style_bg_color(menu, lv_color_make( 13, 17, 23 ), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color( menu , lv_color_make( 40, 52, 71 ), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( menu, LV_GRAD_DIR_VER, LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_bg_color(menu, lv_color_make(255, 211, 165), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color( menu , lv_color_make(213, 145, 142), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( menu, LV_GRAD_DIR_VER, LV_PART_MAIN );
    }

    /*Store Background color of menu in a variable*/
    lv_color_t bgColor = lv_obj_get_style_bg_color(menu, LV_PART_MAIN);

    /*Adjust color based on brightness of color*/
    if(lv_color_brightness(bgColor) > 127)
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(bgColor, 10), LV_PART_MAIN);
    }
    else
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(bgColor, 50), LV_PART_MAIN);
    }

    /*Set size and align menu*/
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL) - 20);

    /*Align to bottom mid, so timedate header is at top*/
    lv_obj_align(menu, LV_ALIGN_BOTTOM_MID, 0, 0);

    /*SUB PAGE CREATION-------------------------------------------------------------------------------------------------------------------------*/



    /*DATE PAGE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    /*Create Date Page on the Menu*/
    lv_obj_t *datePage = lv_menu_page_create(menu, NULL);

    /*Set Horizontal Padding equal to Header of the page*/
    lv_obj_set_style_pad_hor(datePage, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), LV_PART_MAIN), LV_PART_MAIN);

    /*Create a separator, separates sections, in this case separates calendar from page header*/
    lv_menu_separator_create(datePage);

    /*Create and load Calendar Data variable*/
    calendarData_t calendarData;
    calendarData.date = settingpagedata->date;
    calendarData.day = settingpagedata->day;
    calendarData.month = settingpagedata->month;
    calendarData.year = settingpagedata->year;
    
    /*Create Calendar Widget*/
    lv_obj_t *calendar = createCalendar(datePage, &calendarData);
    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_set_style_bg_color(calendar, lv_color_make( 13, 17, 23 ), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color( calendar , lv_color_make( 40, 52, 71 ), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( calendar, LV_GRAD_DIR_VER, LV_PART_MAIN );
        lv_obj_set_style_text_color( calendar, lv_color_white(), LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_bg_color(calendar, lv_color_make(255, 211, 165), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color( calendar , lv_color_make(213, 145, 142), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( calendar, LV_GRAD_DIR_VER, LV_PART_MAIN );
        lv_obj_set_style_border_color( calendar, lv_color_black(), LV_PART_MAIN );
    }

    /*Create a separator, separates sections, in this case separates save button from calendar*/
    lv_menu_separator_create(datePage);

    /*Create 'Save' Button*/
    guiBtnStyleInit( gui_element );
    createSaveBtn(datePage,"Save",&gui_element->styleBtnNormal,&gui_element->styleBtnClicked,eventHandlerDateSave);
    /*END DATE PAGE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    


    /*TIME PAGE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    /*Create Time Page*/
    lv_obj_t *timePage = lv_menu_page_create(menu, NULL);

    /*Set Horizontal padding of page equal to header of page*/
    lv_obj_set_style_pad_hor(timePage, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), LV_PART_MAIN), LV_PART_MAIN);

    /*Seperate from header*/
    lv_menu_separator_create(timePage);

    /*Create section*/
    section = lv_menu_section_create(timePage);
    lv_obj_set_style_bg_opa( section, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Create Roller data placeholders, since there will be 3 rollers, one for hour, minute and second
    Make the roller static since we want the roller to retain values between calls to this function*/
    static rollerData_t rollerTimeHour, rollerTimeMinute, rollerTimeSecond;

    /*Set active ID's for rollers*/
    rollerTimeHour.rollerActiveVal = settingpagedata->clockHour - 1; //Subtract one since roller hour starts at 1, when clock data starts from 0
    rollerTimeMinute.rollerActiveVal = settingpagedata->clockMin;
    rollerTimeSecond.rollerActiveVal = settingpagedata->clockSecond;

    /*Create Parent row*/
    parentRow = createSettingsTimeAlarmRollersRow(section, &rollerTimeHour, &rollerTimeMinute, &rollerTimeSecond, eventHandlerTimeRollers);

    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_set_style_bg_opa( parentRow, LV_OPA_TRANSP, LV_PART_MAIN );
        lv_obj_set_style_text_color( parentRow, lv_color_white(), LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_bg_opa( parentRow, LV_OPA_TRANSP, LV_PART_MAIN );
        lv_obj_set_style_border_color( parentRow, lv_color_black(), LV_PART_MAIN );
    }

    /*Create CheckBox Data type, make them static so they retain values between calls to this function*/
    static CheckBoxData_t meridiemFormat; 

    /*Set CheckBox Data Setting Type*/
    meridiemFormat.CheckBoxSettingType = SETTING_TIME;

    /*Adjust Initial CheckBox Data based on Clock Format*/
    if( settingpagedata->clockFormat == FORMAT_AM )
    {
        /*Set CheckBox Active Value*/
        meridiemFormat.CheckBoxBoxIndex = CHECK_BOX_INDEX_AM;
    }
    else if( settingpagedata->clockFormat == FORMAT_PM )
    {
        /*Set CheckBox active value*/
        meridiemFormat.CheckBoxBoxIndex = CHECK_BOX_INDEX_PM;
    }

    /*Create Time Format CheckBoxes*/
    createMeridiemCheckBoxes(parentRow, &meridiemFormat, gui_element);

    /*Create a seperator, separates the parentRow from the 24 Hour Switch*/
    lv_menu_separator_create(timePage);

    /*Create New section for 24 Hour switch*/
    section = lv_menu_section_create(timePage);
    lv_obj_set_style_bg_opa( section, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Create 12H24H Switch */
    lv_obj_t *switch12H24H = createONOFFSwitch(section, LV_SYMBOL_SETTINGS, "24H", false, eventHandler12H24HSwitch);
    
    /*Set the state of the meridiem switch based on current mode*/
    if( settingpagedata->clock12H24H == MODE_12H )
    {
        /*Set the switch off*/
        lv_obj_add_state( switch12H24H, LV_STATE_DEFAULT );
    }
    else if( settingpagedata->clock12H24H == MODE_24H )
    {
        /*Turn on the switch*/
        lv_obj_add_state( switch12H24H, LV_STATE_CHECKED );
    }

    /*Create separator, separates 24hour switch from 'Save' button*/
    lv_menu_separator_create( timePage );

    /*Create Save Button with Time Change Handler*/
    createSaveBtn(timePage, "Save", &gui_element->styleBtnNormal, &gui_element->styleBtnClicked, eventHandlerTimeSave );  
    /*END TIME PAGE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/



    /*ALARM PAGE////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    /*Create Alarm Page*/
    lv_obj_t *alarmPage = lv_menu_page_create(menu, NULL);

    /*Set Horizontal padding of page equal to header of page*/
    lv_obj_set_style_pad_hor(alarmPage, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), LV_PART_MAIN), LV_PART_MAIN);

    /*Seperate from header*/
    lv_menu_separator_create(alarmPage);

    /*Create section*/
    section = lv_menu_section_create(alarmPage);
    lv_obj_set_style_bg_opa( section, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Create Roller data placeholders, since there will be 2 rollers, one for hour and minute
    Make the roller static since we want the roller to retain values between calls to this function*/
    static rollerData_t rollerAlarmHour, rollerAlarmMinute;

    /*Set active ID's for Alarm Rollers*/
    rollerAlarmHour.rollerActiveVal = settingpagedata->alarmHour - 1; //Subtract 1 since the roller hour starts at 1, when the alarm data starts at 0
    rollerAlarmMinute.rollerActiveVal = settingpagedata->alarmMinute;

    /*Create Parent row*/
    parentRow = createSettingsTimeAlarmRollersRow( section, &rollerAlarmHour, &rollerAlarmMinute, NULL, eventHandlerAlarmRollers );

    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_set_style_bg_opa( parentRow, LV_OPA_TRANSP, LV_PART_MAIN );
        lv_obj_set_style_text_color( parentRow, lv_color_white(), LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_bg_opa( parentRow, LV_OPA_TRANSP, LV_PART_MAIN );
        lv_obj_set_style_border_color( parentRow, lv_color_black(), LV_PART_MAIN );
    }

    /*Create checkbox data type for Alarm, make it static so it retains values between calls to this function*/
    static CheckBoxData_t alarmFormat;

    /*Set checkbox data type*/
    alarmFormat.CheckBoxSettingType = SETTING_ALARM;

    /*Adjust initial value of checkbox based on Alarm Format*/
    if( settingpagedata->clockFormat == FORMAT_AM )
    {
        /*Set Checkbox Acive value*/
        alarmFormat.CheckBoxBoxIndex = CHECK_BOX_INDEX_AM;
    }
    else if( settingpagedata->clockFormat == FORMAT_PM )
    {
        /*Set CheckBox active value*/
        alarmFormat.CheckBoxBoxIndex = CHECK_BOX_INDEX_PM;
    }

    /*Create Alarm Check Boxes*/
    createMeridiemCheckBoxes( parentRow, &alarmFormat, gui_element);

    /*Create Variables to store Alarm ON/OFF status and text of the Alarm lable*/
    char *alarmTxt = NULL;
    bool alarmCurrVal = false;

    /*Set Alarm Text and Current val based on current data*/
    if( settingpagedata->alarmStatus == ALARM_ON )
    {
        alarmTxt = "Alarm : ON";
        alarmCurrVal = true;
    }
    else if( settingpagedata->alarmStatus == ALARM_OFF )
    {
        alarmTxt = "Alarm : OFF";
        alarmCurrVal = false;
    }

    /*Create separator, separates the parent row from the Alarm ON/OFF switch label*/
    lv_menu_separator_create(alarmPage);

    /*Create a section for the Alarm ON/OFF lable*/
    section = lv_menu_section_create( alarmPage );
    lv_obj_set_style_bg_opa( section, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Create the ON/OFF Switch on the section*/
    container = createONOFFSwitch( section, LV_SYMBOL_BELL, alarmTxt, alarmCurrVal, eventHandlerAlarmONOFFSwitch );
    lv_obj_set_style_border_color( container, lv_color_black(), LV_PART_MAIN );
    /*END ALARM PAGE////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/



    /*DISPLAY PAGE//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    /*Create Display page*/
    lv_obj_t *displayPage = lv_menu_page_create( menu, NULL );

    /*Set Horizontal padding of the page equal to the header of the page*/
    lv_obj_set_style_pad_hor( displayPage, lv_obj_get_style_pad_left( lv_menu_get_main_header( menu ), LV_PART_MAIN ), LV_PART_MAIN );

    /*Create separator to separate content from header*/
    lv_menu_separator_create( displayPage );

    /*Create Section*/
    section = lv_menu_section_create( displayPage );
    lv_obj_set_style_bg_opa( section, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Create Display slider*/
    createDisplaySlider( section, LV_SYMBOL_SETTINGS, "Brightness", 0, 150, 100, eventHandlerDisplaySlider );
    /*END DISPLAY PAGE//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/



    /*ROOT PAGE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    /*Create Root page*/
    rootPage = lv_menu_page_create(menu, "Settings");

    /*Create section on Root Page for the all the menu options*/
    section = lv_menu_section_create(rootPage);

    if( gui_element->theme == THEME_DARK )
    {
        /*Date Settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "#FFFFFF     Date         Date", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, datePage );
        defaultcontainerToShow = container;

        /*Time Settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "#FFFFFF    Time         Time ", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, timePage );

        /*Alarm Settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "#FFFFFF    Alarm        Alarm", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, alarmPage );

        /*Display settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "#FFFFFF    Display      Display", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, displayPage );

        /*Set the Root page as sidebar page for menu*/
        lv_menu_set_sidebar_page(menu, rootPage);

        lv_obj_set_style_text_color( lv_menu_get_sidebar_header_back_btn( menu ), lv_color_white( ), LV_PART_MAIN );
        lv_obj_set_style_text_color( lv_menu_get_sidebar_header( menu ), lv_color_white( ), LV_PART_MAIN );

    }
    else
    {
         /*Date Settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "     Date         Date", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, datePage );
        defaultcontainerToShow = container;

        /*Time Settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "    Time         Time ", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, timePage );

        /*Alarm Settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "   Alarm        Alarm", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, alarmPage );

        /*Display settings*/
        container = createLabelContainer( rootPage, LV_SYMBOL_SETTINGS, "    Display      Display", LV_MENU_ITEM_BUILDER_VARIANT_1 );
        lv_menu_set_load_page_event( menu, container, displayPage );

        /*Set the Root page as sidebar page for menu*/
        lv_menu_set_sidebar_page(menu, rootPage);
    }

    /*Manually send event for Default Container to show*/
    lv_event_send( defaultcontainerToShow, LV_EVENT_CLICKED, NULL);
    /*END ROOT PAGE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
}

/**
 * @brief Clear Screen(All child objects) and remove Drop Down List
 * 
 * @param gui_element 
 */
void screenCleanup(GUI_t *const gui_element)
{
    lv_obj_clean(gui_element->screen);
    gui_element->dropDownList = NULL;
}

/**
 * @brief Initialize style for main page of GUI
 * 
 * @param gui_element GUI_t member of the main ClockAlarmUI_t object, i.e. gui_inst
 */
void guiCreateMainPageStyle(GUI_t *const gui_element)
{
    /*Set background color of main page based on current theme */
    if( gui_element->theme == THEME_LIGHT )
    {
        lv_obj_set_style_bg_color(gui_element->screen, lv_color_make(255, 211, 165), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(gui_element->screen, lv_color_make(213, 145, 142), LV_PART_MAIN);
    }
    else
    {
        lv_obj_set_style_bg_color(gui_element->screen, lv_color_make( 13, 17, 23 ), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(gui_element->screen, lv_color_make( 40, 52, 71 ), LV_PART_MAIN);
    }

    lv_obj_set_style_bg_grad_dir(gui_element->screen, LV_GRAD_DIR_VER, LV_PART_MAIN);
    
    /*Set clock text font*/
    lv_obj_set_style_text_font(gui_element->clock, &lv_font_montserrat_40, LV_PART_MAIN);

    /*Set Various Date elements fonts*/
    lv_obj_set_style_text_font(gui_element->day, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_font(gui_element->month, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_font(gui_element->date, &lv_font_montserrat_40, LV_PART_MAIN);
    lv_obj_set_style_text_font(gui_element->year, &lv_font_montserrat_12, LV_PART_MAIN);

    /*Clock Alignment using percentanges*/
    lv_obj_align(gui_element->clock, LV_ALIGN_TOP_RIGHT, lv_pct(-5), lv_pct(40));

    /*Set various date elements alignment*/
    lv_obj_align(gui_element->clock, LV_ALIGN_TOP_RIGHT, LV_PCT(-5), LV_PCT(30));
    lv_obj_align(gui_element->date, LV_ALIGN_TOP_LEFT, LV_PCT(5), LV_PCT(40));
    lv_obj_align(gui_element->day, LV_ALIGN_TOP_LEFT, LV_PCT(5), LV_PCT(35));
    lv_obj_align(gui_element->month, LV_ALIGN_TOP_LEFT, LV_PCT(5), LV_PCT(30));
    lv_obj_align(gui_element->year, LV_ALIGN_TOP_LEFT, LV_PCT(5), LV_PCT(55));  

    /*Dropdown Button Setup-----------------------------------------------------------------------------------------------------*/

    /*Set Btn size and alignment*/
    lv_obj_set_size(gui_element->dropDownBtn, 30, 30);
    lv_obj_align(gui_element->dropDownBtn, LV_ALIGN_TOP_RIGHT, 0, 0);

    /*Set background opacity to 90% ONLY when pressed*/
    lv_obj_set_style_bg_opa(gui_element->dropDownBtn, LV_OPA_TRANSP, LV_STATE_PRESSED);
    
    /*Set corner radius to 5 pixels ONLY when pressed*/
    lv_obj_set_style_radius(gui_element->dropDownBtn, 5, LV_STATE_PRESSED);

    /*Set background color to Deep purple(2 shades darker) ONLY when pressed*/
    lv_obj_set_style_bg_color(gui_element->dropDownBtn, lv_palette_darken(LV_PALETTE_DEEP_PURPLE, 2), LV_STATE_PRESSED);
    
    /*Create a label, using Drop Down btn as Parent*/
    lv_obj_t* label = lv_label_create(gui_element->dropDownBtn);

    /*Set the icon and icon color for dropdown label*/
    lv_label_set_text(label, LV_SYMBOL_LIST);
    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_set_style_text_color(label, lv_color_white(), 0);
    }
    lv_obj_set_style_text_font(label, &lv_font_montserrat_30, LV_PART_MAIN);

    /*Center the label to center of its parent(droppdwnbtn)*/
    lv_obj_center(label);        
}

/**
 * @brief Create and assign label objects to each element of gui_inst of main object, assign callback to screen and dropDownBtn
 * 
 * @param gui_element GUI_t member of the main ClockAlarmUI_t object, i.e. gui_inst
 */
void guiCreateMainPageLabels(GUI_t *const gui_element)
{
    /*Create label object and assign to clock element of gui_element*/
    gui_element->clock = lv_label_create(gui_element->screen);

    /*Create label objects and assign to date elements of gui_element*/
    gui_element->day = lv_label_create(gui_element->screen);
    gui_element->month = lv_label_create(gui_element->screen);
    gui_element->year = lv_label_create(gui_element->screen);
    gui_element->date = lv_label_create(gui_element->screen);

    if( gui_element->theme == THEME_DARK )
    {
    lv_obj_set_style_text_color(gui_element->day, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_color(gui_element->day, lv_color_white(), LV_PART_MAIN);
    gui_element->month = lv_label_create(gui_element->screen);
        lv_obj_set_style_text_color(gui_element->day, lv_color_white(), LV_PART_MAIN);
    gui_element->month = lv_label_create(gui_element->screen);
    lv_obj_set_style_text_color(gui_element->month, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_color(gui_element->month, lv_color_white(), LV_PART_MAIN);
    gui_element->year = lv_label_create(gui_element->screen);
        lv_obj_set_style_text_color(gui_element->month, lv_color_white(), LV_PART_MAIN);
    gui_element->year = lv_label_create(gui_element->screen);
    lv_obj_set_style_text_color(gui_element->year, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_color(gui_element->year, lv_color_white(), LV_PART_MAIN);
    gui_element->date = lv_label_create(gui_element->screen);
        lv_obj_set_style_text_color(gui_element->year, lv_color_white(), LV_PART_MAIN);
    gui_element->date = lv_label_create(gui_element->screen);
        lv_obj_set_style_text_color(gui_element->date, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_color(gui_element->clock, lv_color_white(), LV_PART_MAIN);
    }

    /*Create button for drop down button on main screen*/
    gui_element->dropDownBtn = lv_btn_create(gui_element->screen);

    /*Remove all the syle properties, so button exits, but cannot be seen and has no style elements, style init @fn guiCreateMainPageStyle*/
    lv_obj_remove_style_all(gui_element->dropDownBtn);

    /*Add callback for screen*/
    lv_obj_add_event_cb(gui_element->screen, eventHandlerScreen, LV_EVENT_PRESSED, (void *)gui_element);
    
    /*Add callback for drop down button*/
    lv_obj_add_event_cb(gui_element->dropDownBtn, eventHandlerDropDownBtn, LV_EVENT_CLICKED, (void *)gui_element);

    /*Create Theme Switch, align and add callback*/
    gui_element->themeSwitch = lv_switch_create( gui_element->screen );
    lv_obj_set_style_bg_opa( gui_element->themeSwitch, LV_OPA_0, LV_PART_INDICATOR | LV_STATE_CHECKED );
    lv_obj_align( gui_element->themeSwitch, LV_ALIGN_BOTTOM_RIGHT, lv_pct(-5), lv_pct( -5) );  
    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_add_state( gui_element->themeSwitch, LV_STATE_CHECKED );
    }
    lv_obj_add_event_cb( gui_element->themeSwitch, eventHandlerThemeSwitch, LV_EVENT_VALUE_CHANGED, NULL );

    /*Style Theme Switch Normal*/
    lv_obj_set_style_bg_opa( gui_element->themeSwitch, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_border_opa( gui_element->themeSwitch, LV_OPA_100, LV_PART_MAIN | LV_PART_INDICATOR);
    lv_obj_set_style_border_width( gui_element->themeSwitch, 2, LV_PART_MAIN | LV_PART_INDICATOR);
    lv_obj_set_style_outline_opa( gui_element->themeSwitch, LV_OPA_100, LV_PART_MAIN );
    lv_obj_set_style_bg_opa( gui_element->themeSwitch, LV_OPA_0, LV_PART_INDICATOR | LV_STATE_CHECKED );

    if( gui_element->theme == THEME_DARK )
    {
        lv_obj_set_style_border_color( gui_element->themeSwitch, lv_color_white(), LV_PART_MAIN | LV_PART_INDICATOR);

    }
    else
    {
        lv_obj_set_style_bg_color( gui_element->themeSwitch, lv_color_black(), LV_PART_KNOB );
    }
}

/**
 * @brief Style Init for the 'Save' Btn with ripple effect
 * 
 * @param gui_element GUI_t member of the main ClockAlarmUI_t object, i.e. gui_inst
 */
void guiBtnStyleInit(GUI_t *const gui_element)
{
    /*Initialize default style for normal and clicked*/
    lv_style_init(&gui_element->styleBtnNormal);
    lv_style_init(&gui_element->styleBtnClicked);
    lv_style_init( &gui_element->styleThemeChecked );
    gui_element->lightThemeColor = lv_color_make(255, 211, 165);
    gui_element->darkThemeColor = lv_color_make( 13, 17, 23 );

    /*Border vs Outline
    Border - increasing the border width on a rectangle will consume more of its interior space
    Outline - increasing the outline width expands outward without altering the interior container of the object*/

    /*Normal Btn----------------------------------------------------------------------------------------------------------*/
    /*Btn corner radius*/
    lv_style_set_radius(&gui_element->styleBtnNormal, 15);
    
    /*Background Opacity*/
    lv_style_set_bg_opa(&gui_element->styleBtnNormal, LV_OPA_TRANSP);
    
    /*Background Color*/
    //lv_style_set_bg_color(&gui_element->styleBtnNormal, lv_color_make(11, 11, 69));

    /*Border Opacity*/
    //lv_style_set_bg_opa(&gui_element->styleBtnNormal, LV_OPA_40);

    /*Border width*/
    lv_style_set_border_width(&gui_element->styleBtnNormal, 2);

    /*Outline Opacity*/
    lv_style_set_outline_opa(&gui_element->styleBtnNormal, LV_OPA_100);
    
    /*Outline color*/
    if( gui_element->theme == THEME_DARK )
    {
        lv_style_set_outline_color(&gui_element->styleBtnNormal, gui_element->lightThemeColor);
        /*Border color*/
        lv_style_set_border_color(&gui_element->styleBtnNormal, lv_color_white() );
        lv_style_set_text_color(&gui_element->styleBtnNormal, lv_color_white());
    }
    else
    {
        lv_style_set_outline_color(&gui_element->styleBtnNormal, gui_element->darkThemeColor);
        /*Border color*/
        lv_style_set_border_color(&gui_element->styleBtnNormal, lv_color_black() );
    }
    
    
    /*Set padding, increase distance from text to edge of container in all directions by 10 pixels*/
    lv_style_set_pad_all(&gui_element->styleBtnNormal, 10);

    /*Clicked Btn---------------------------------------------------------------------------------------------------------*/
    /*Set outline width when pressed*/
    lv_style_set_outline_width(&gui_element->styleBtnClicked, 30);

    /*Outline opacity when pressed*/
    lv_style_set_outline_opa(&gui_element->styleBtnClicked, LV_OPA_TRANSP);    

    /*Add slight vertical translation after pressing*/
    lv_style_set_translate_y(&gui_element->styleBtnClicked, 5);

    if( gui_element->theme == THEME_DARK )
    {
        lv_style_set_bg_color(&gui_element->styleBtnClicked, lv_color_white() );
        lv_style_set_bg_grad_color(&gui_element->styleBtnClicked, lv_color_black());
        //lv_style_set_shadow_color( &gui_element->styleBtnClicked, lv_color_make( 40, 52, 71 ) );
    }
    else
    {
        lv_style_set_bg_color(&gui_element->styleBtnClicked, lv_color_make( 13, 17, 23 ));
        lv_style_set_bg_grad_color(&gui_element->styleBtnClicked, lv_color_make(138, 138, 162));
        lv_style_set_shadow_color( &gui_element->styleBtnClicked, gui_element->darkThemeColor );
        lv_style_set_text_color(&gui_element->styleBtnClicked, lv_color_white());
    }

    /*Set gradient direction*/
    lv_style_set_bg_grad_dir(&gui_element->styleBtnClicked, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa( &gui_element->styleBtnClicked, LV_OPA_100 );

    lv_style_set_shadow_ofs_x( &gui_element->styleBtnClicked, 5);
    lv_style_set_shadow_ofs_y( &gui_element->styleBtnClicked, 10);
    lv_style_set_shadow_width( &gui_element->styleBtnClicked, 20);
    

    /*Transition-------------------------------------------------------------------------------------------------------------*/
    /*Create a transition description object*/
    static lv_style_transition_dsc_t transition;
    
    /*create an arry of Which things(properties) do we want to change during the transition*/
    static lv_style_prop_t properties[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA};

    /*Initialize transition description object with above properties*/
    lv_style_transition_dsc_init(&transition, properties, lv_anim_path_linear, 325, 0, NULL);

    /*Set the transition to the style: lv_Btn_style_clicked*/
    lv_style_set_transition(&gui_element->styleBtnClicked, &transition);
}

/**
 * @brief Initialize default settings for clock style and call Btn style init
 * 
 * @param gui_element GUI_t member of the main ClockAlarmUI_t object, i.e. gui_inst
 */
void lvglStyleInit(GUI_t *const gui_element)
{
    /*Pass in the address(gui_element->styleClock) because that address points to lv_style_t *styleClock, gui_element->styleClock returns the
    element itself not a pointer to the element. If we pass in the element, since its pass by copy, we cant change the original element, we will
    just end up changing a copy of the element */
    
    /*Initialize default Style for the styleClock element of the GUI_t structure, which is an element of the main object*/
    lv_style_init(&gui_element->styleClock);

    /*Initialize style for 'Save' Btn object*/
    guiBtnStyleInit(gui_element);
}

/**
 * @brief Set the screen element of gui_element of main object to current active screen and call LVGL style init func for button
 * 
 *  @param gui_element GUI_t member of the main ClockAlarmUI_t object, i.e. gui_inst
 */
void guiStyleCreator(GUI_t *const gui_element)
{
    /*Set screen member of gui_element = current active screen*/
    gui_element->screen = lv_scr_act();

    /*Set Initial Theme as Light*/
    gui_element->theme = THEME_LIGHT;

    /*Initialize LVGL Style*/
    lvglStyleInit(gui_element);
}

