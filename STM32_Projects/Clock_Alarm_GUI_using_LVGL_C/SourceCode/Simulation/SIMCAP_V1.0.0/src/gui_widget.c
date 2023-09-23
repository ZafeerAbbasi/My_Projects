/*
# ##############################################################################
# File: gui_widget.c                                                           #
# Project: src                                                                 #
# Created Date: Tuesday, August 29th 2023, 7:31:05 pm                          #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Friday, September 22nd 2023, 9:18:30 pm                       #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/



/*##############################################################################################################################################*/
/*INCLUDES______________________________________________________________________________________________________________________________________*/
/*##############################################################################################################################################*/

#include "lvgl.h"
#include "project_clock_alarm.h"
#include "gui_widget.h"
#include "stdlib.h"
#include "examples/lv_examples.h"
#include "stdio.h"

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
 * @brief Create a Message Box object ( The buttons are automatically bubbled )
 * 
 * @param parent pointer to parent or NULL to create a full screen modal message box
 * @param title the title of the message box
 * @param msg the text of the message box
 * @param opts the buttons as an array of texts terminated by an "" element. E.g. {"btn1", "btn2", ""}
 * @param closeBtn add a close button
 * @return lv_obj_t* pointer to the message box object
 */
lv_obj_t *createMessageBox( lv_obj_t *parent, const char *title, const char *msg, const char*opts[], bool closeBtn )
{
    /*Create Message Box*/
    lv_obj_t *messageBox = lv_msgbox_create( parent, title, msg, opts, closeBtn );

    /*Center the Message Box*/
    lv_obj_center( messageBox );

    /*Add callback*/
    lv_obj_add_event_cb( messageBox, eventHandlerMessageBox, LV_EVENT_VALUE_CHANGED, NULL );

    return messageBox;
}

/**
 * @brief Create a Slider object
 * 
 * @param parent Parent to place the Slider on
 * @param icon Icon used for Slider Label
 * @param txt Text used for Slider label
 * @param min Minimum Value for the slider
 * @param max Maximum Value for the slider
 * @param currVal Current Value to set for the slider
 * @return lv_obj_t* Pointer to the created slider
 */
lv_obj_t *createDisplaySlider( lv_obj_t *parent, const char *icon, const char*txt, int32_t min, int32_t max, int32_t currVal, lv_event_cb_t eventCallBack )
{
    /*Create Label Container*/
    lv_obj_t *sliderLabelContainer = createLabelContainer( parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2 );
    //lv_obj_set_style_opa( sliderLabelContainer, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Create slider object*/
    lv_obj_t *slider = lv_slider_create( sliderLabelContainer );

    /*Set Flex growth of slider*/
    lv_obj_set_flex_grow( slider, 1 );

    /*Set Range and Current value*/
    lv_slider_set_range( slider, min, max );
    lv_slider_set_value( slider, currVal, LV_ANIM_ON );

    if( icon == NULL )
    {
        /*IF icon is null then start new flow from slider*/
        lv_obj_add_flag( slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK );
    }

    /*Create New container for the slider percentage*/
    lv_obj_t *sliderPercentContainer = lv_menu_cont_create( parent );

    /*Set Flex Align of the Percent Container*/
    lv_obj_set_flex_align( sliderPercentContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER );

    /*Create Label below the slider*/
    lv_obj_t *sliderPercentLabel = lv_label_create( sliderPercentContainer );

    /*Add flag to start new line of label*/
    lv_obj_add_flag( sliderPercentLabel, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK );

    /*Add current percent and align*/
    lv_label_set_text_fmt( sliderPercentLabel, "%d%%", lv_slider_get_value( slider ) );

    if( clockAlarmUI_inst.gui_inst.theme == THEME_DARK )
    {
        lv_obj_set_style_text_color( sliderPercentLabel, lv_color_white(), LV_PART_MAIN );
    }

    /*Align label*/
    lv_obj_align_to( sliderPercentLabel, slider, LV_ALIGN_BOTTOM_MID, 0, 10 );

    /*Add callback to slider*/
    lv_obj_add_event_cb( slider, eventCallBack, LV_EVENT_VALUE_CHANGED, sliderPercentLabel );

    return slider;
}

/**
 * @brief Create a Label Container object
 * 
 * @param parent Parent to place the label on
 * @param icon Icon used in the Label
 * @param txt Text for the Label
 * @param builderVariant Builder Variant
 * @return lv_obj_t* Pointer to the created label object
 */
lv_obj_t *createLabelContainer(lv_obj_t *parent, const char *icon, const char *txt, menuItemBuilderVariant_t builderVariant)
{
    /*Create container for the Label on the Menu Section*/
    lv_obj_t *container = lv_menu_cont_create(parent);

    /*Placeholders for image and label pointers*/
    lv_obj_t *img = NULL;
    lv_obj_t *label = NULL;

    if( icon )
    {
        /*If icon param is valid*/
        /*create image*/
        img = lv_img_create(container);
        
        if( clockAlarmUI_inst.gui_inst.theme == THEME_DARK )
        {
            lv_obj_set_style_text_color( img, lv_color_white(), LV_PART_MAIN);
        }

        /*Set source for img, i.e. the icon*/
        lv_img_set_src(img, icon);
    }

    if( txt )
    {
        /*If txt param is valid*/
        /*Create lable*/
        label = lv_label_create(container);
        lv_label_set_recolor( label, true );

        /*Set label text*/
        lv_label_set_text( label, txt );

        if( clockAlarmUI_inst.gui_inst.theme == THEME_DARK )
        {
            lv_obj_set_style_text_color( label, lv_color_white(), LV_PART_MAIN);
        }

        /*Set long mode, so circular scroll when text is too long*/
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);

        /*Set Flex Grow of Label so it fills the whitespace
        Explanation: Usage:
        If you have a flex container with two children, and you set the "flex grow" value of the first child to 1 
        and the second child to 2, the second child will take up twice as much of the extra space as the first child.*/
        lv_obj_set_flex_grow(label, 1);
    }

    if( builderVariant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt )
    {
        /*Add Flag Flex in New Track Flag
        Explanation: The LV_OBJ_FLAG_FLEX_IN_NEW_TRACK flag is used to indicate that an object inside a flex 
        container should start a new track (line or column, depending on the flex direction). This is especially 
        useful when you're working with flex layouts that wrap, and you want to force a particular object to 
        start on a new line or column, regardless of the available space in the current track*/
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);

    }



    return container;
}

/**
 * @brief Create a ON/OFF Switch
 * 
 * @param parent Parent to place the Meridiem Switch on
 * @param icon Icon to add to the switch label
 * @param txt Text for the switch label 
 * @param currSwitchVal Should the switch be checked
 * @param eventCallBack Callback for the switch
 * @return lv_obj_t* Pointer to the created switch
 */
lv_obj_t *createONOFFSwitch(lv_obj_t *parent, const char *icon, const char *txt, bool currSwitchVal, lv_event_cb_t eventCallBack)
{
    /*Create Container with Label*/
    lv_obj_t *labelContainer1 = createLabelContainer(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_obj_set_style_bg_opa( labelContainer1, LV_OPA_TRANSP, LV_PART_MAIN );

    /*Create switch on label object*/
    lv_obj_t *ONOFFSwitch = lv_switch_create(labelContainer1);

    /*Add state based on currSwitchVal*/
    lv_obj_add_state( ONOFFSwitch, ( currSwitchVal ? LV_STATE_CHECKED : 0 ) );

    /*Get Label from labelContainer, 0 is img, 1 is label*/
    lv_obj_t *ONOFFSwitchLabel = lv_obj_get_child( labelContainer1, 1 );

    /*Add event callback to meridiem switch*/
    lv_obj_add_event_cb( ONOFFSwitch, eventCallBack, LV_EVENT_VALUE_CHANGED, ONOFFSwitchLabel );

    return ONOFFSwitch;
}

/**
 * @brief Create a Check Box object
 * 
 * @param parent Parent to place the child Check Boxes on 
 * @param txt Text to Add to Check Box
 * @param styleCheckBox Style for the normal Check Box
 * @param styleCheckBoxSelected Style for when the Check Box is checked
 * @return lv_obj_t* Pointer to the created checkbox
 */
lv_obj_t * createCheckBox(lv_obj_t *parent, const char *txt, lv_style_t *styleCheckBox, lv_style_t *styleCheckBoxSelected)
{
    /*Create Check box*/
    lv_obj_t *checkBox = lv_checkbox_create(parent);

    /*Set text*/
    lv_checkbox_set_text(checkBox, txt);

    if( clockAlarmUI_inst.gui_inst.theme == THEME_DARK )
    {
        lv_obj_set_style_text_color( checkBox, lv_color_white(), LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_text_color( checkBox, lv_color_black(), LV_PART_MAIN );
    }

    /*Add Flag to make the box bubbled so an event from the checkbox causes a event for the parent container*/
    lv_obj_add_flag(checkBox, LV_OBJ_FLAG_EVENT_BUBBLE);

    return checkBox;
}

/**
 * @brief Create a Roller object
 * 
 * @param parent Parent to place child roller object on
 * @param opts String containing possible options for roller selections
 * @param currVal Default value selection for roller
 * @return lv_obj_t* Pointer to Roller object
 */
lv_obj_t *createRoller(lv_obj_t *parent, const char *opts, int currVal)
{
    /*Create a plain object*/
    lv_obj_t *roller;

    /*Create a style for the main selected option of the roller, i.e. the middle option*/
    static lv_style_t rollerStyle;

    /*Initialize the style, (set to default values)*/
    lv_style_init(&rollerStyle);

    /*Set style text*/
    lv_style_set_text_font(&rollerStyle, &lv_font_montserrat_22);

    /*Create a roller object and assign it to the previous plain object that was created*/
    roller = lv_roller_create(parent);

    /*Set roller options and mode, normal = normal, infinite = infinite scroll*/
    lv_roller_set_options(roller, opts, LV_ROLLER_MODE_NORMAL);

    /*Set visible rows, if chaning this then make sure to adjust line below: lv_obj_set_height(roller, LV_PCT(100))*/
    /*The way the below function sets the visible row count is by adjusting the height of the roller, however
    we manually adjust the height ourselves after, thus this function is functionally useless*/
    //lv_roller_set_visible_row_count(roller, 2);

    /*Set the current selected option on the roller*/
    lv_roller_set_selected(roller, currVal, LV_ANIM_ON);

    /*Set height and width of roller*/
    lv_obj_set_height(roller, LV_PCT(100)); //Set hieght of the roller to the height of the container object*/
    lv_obj_set_width(roller, 60);

    /*Add style to the current middle selected option of the roller*/
    lv_obj_add_style(roller, &rollerStyle, LV_PART_SELECTED);
    lv_obj_set_style_bg_opa( roller, LV_OPA_TRANSP, LV_PART_SELECTED );

    /*Align roller option text to the left*/
    lv_style_set_text_align(&rollerStyle, LV_TEXT_ALIGN_LEFT);

    if( clockAlarmUI_inst.gui_inst.theme == THEME_DARK )
    {
        lv_obj_set_style_border_color( roller, lv_color_white(), LV_PART_MAIN );
        lv_obj_set_style_text_color( roller, lv_color_white(), LV_PART_SELECTED );
        lv_obj_set_style_text_color( roller, lv_color_white(), LV_PART_MAIN );

    }
    else
    {
        lv_obj_set_style_border_color( roller, lv_color_black(), LV_PART_MAIN );
        lv_obj_set_style_text_color( roller, lv_color_black(), LV_PART_SELECTED );
        lv_obj_set_style_text_color( roller, lv_color_black(), LV_PART_MAIN );
    }

    return roller;
}

/**
 * @brief Create a Save Btn object
 * 
 * @param parent Parent to place child Save Button on 
 * @param txt Text on the Button
 * @param normal Style of the button Normally
 * @param clicked Style of the Button when clicked
 * @param eventCallBack Callback when the button is pressed
 * @return lv_obj_t* Pointer to 'Save' Button object
 */
lv_obj_t *createSaveBtn(lv_obj_t *parent, const char* txt, lv_style_t *normal, lv_style_t *clicked, lv_event_cb_t eventCallBack)
{
    /*Create Button*/
    lv_obj_t *btnObj = lv_btn_create(parent);

    /*Clear all previous styles*/
    lv_obj_remove_style_all(btnObj);

    /*Add normal style to button when normal*/
    lv_obj_add_style(btnObj, normal, LV_STATE_DEFAULT);

    /*Add clicked style to button when clicked*/
    lv_obj_add_style(btnObj, clicked, LV_STATE_PRESSED);

    /*Create label object on top of button, set text and align*/
    lv_obj_t *label = lv_label_create(btnObj);
    lv_label_set_recolor( label, true );
    lv_label_set_text(label, txt);
    lv_obj_center(label);

    /*Add callback on the button*/
    lv_obj_add_event_cb(btnObj, eventCallBack, LV_EVENT_CLICKED, NULL);
}

/**
 * @brief Create a Calendar object
 * 
 * @param parent Parent of which to place the Calendar Child on 
 * @param data Calendar Data used to initialize the Calendar
 * @return lv_obj_t* Pointer to Calendar object
 */
lv_obj_t *createCalendar(lv_obj_t *parent, calendarData_t *data)
{
    /*Create Calendar*/
    lv_obj_t *calendar = lv_calendar_create(parent);

    /*Set size and align*/
    lv_obj_set_size(calendar, 200, 180);
    lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 27);
    
    /*Set Calendar year and month preset*/
    lv_calendar_set_showed_date(calendar, data->year, data->month);

    /*Set Calendar date preset*/
    lv_calendar_set_today_date(calendar, data->year, data->month, data->date);

    /*Use drop down for Year/Month header*/
    lv_obj_t *header = lv_calendar_header_dropdown_create(calendar);
    lv_obj_t *year = lv_obj_get_child( header, 0 );
    lv_obj_t *month = lv_obj_get_child( header, 1 );
    lv_obj_t *yearList = lv_dropdown_get_list( year );
    lv_obj_t *monthList = lv_dropdown_get_list( month );
    lv_obj_set_style_bg_opa( year, LV_OPA_TRANSP, LV_PART_MAIN );
    lv_obj_set_style_bg_opa( month, LV_OPA_TRANSP, LV_PART_MAIN );

    if( clockAlarmUI_inst.gui_inst.theme == THEME_DARK )
    {
        lv_obj_set_style_border_color( year, lv_color_white(), LV_PART_MAIN );
        lv_obj_set_style_border_color( month, lv_color_white(), LV_PART_MAIN );

        lv_obj_set_style_border_color( yearList, lv_color_white(), LV_PART_MAIN );
        lv_obj_set_style_border_color( monthList, lv_color_white(), LV_PART_MAIN );

        lv_obj_set_style_text_color( year, lv_color_white(), LV_PART_MAIN );
        lv_obj_set_style_text_color( month, lv_color_white(), LV_PART_MAIN );

        lv_obj_set_style_text_color( yearList, lv_color_white(), LV_PART_MAIN );
        lv_obj_set_style_text_color( monthList, lv_color_white(), LV_PART_MAIN );

        lv_obj_set_style_bg_color( yearList, lv_color_make( 13, 17, 23 ), LV_PART_MAIN );
        lv_obj_set_style_bg_color( monthList, lv_color_make( 13, 17, 23 ), LV_PART_MAIN );

        lv_obj_set_style_bg_grad_color( yearList, lv_color_make( 40, 52, 71 ), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_color( monthList, lv_color_make( 40, 52, 71 ), LV_PART_MAIN );

        lv_obj_set_style_bg_grad_dir( yearList, LV_GRAD_DIR_VER, LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( monthList, LV_GRAD_DIR_VER, LV_PART_MAIN );
    }
    else
    {
        lv_obj_set_style_border_color( year, lv_color_black(), LV_PART_MAIN );
        lv_obj_set_style_border_color( month, lv_color_black(), LV_PART_MAIN );

        lv_obj_set_style_border_color( yearList, lv_color_black(), LV_PART_MAIN );
        lv_obj_set_style_border_color( monthList, lv_color_black(), LV_PART_MAIN );

        lv_obj_set_style_text_color( year, lv_color_black(), LV_PART_MAIN );
        lv_obj_set_style_text_color( month, lv_color_black(), LV_PART_MAIN );

        lv_obj_set_style_text_color( yearList, lv_color_black(), LV_PART_MAIN );
        lv_obj_set_style_text_color( monthList, lv_color_black(), LV_PART_MAIN );

        lv_obj_set_style_bg_color( yearList, lv_color_make(255, 211, 165), LV_PART_MAIN );
        lv_obj_set_style_bg_color( monthList, lv_color_make(255, 211, 165), LV_PART_MAIN );

        lv_obj_set_style_bg_grad_color( yearList, lv_color_make(213, 145, 142), LV_PART_MAIN );
        lv_obj_set_style_bg_grad_color( monthList, lv_color_make(213, 145, 142), LV_PART_MAIN );

        lv_obj_set_style_bg_grad_dir( yearList, LV_GRAD_DIR_VER, LV_PART_MAIN );
        lv_obj_set_style_bg_grad_dir( monthList, LV_GRAD_DIR_VER, LV_PART_MAIN );
    }

    /*Add Callback event*/
    lv_obj_add_event_cb(calendar, eventHandlerDateChange, LV_EVENT_VALUE_CHANGED, NULL);

    return calendar;
}

/**
 * @brief Create a Menu object
 * 
 * @param parent Parent of which to place the Menu Child on
 * @param enableRootBtn True, or False, Enable Root Button
 * @param eventCallBack Callback for when Root Button is pressed
 * @return lv_obj_t* Pointer to menu object
 */
lv_obj_t *createMenu(lv_obj_t *parent, bool enableRootBtn, lv_event_cb_t eventCallBack)
{
    /*Create menu object*/
    lv_obj_t *menu = lv_menu_create(parent);

    if(enableRootBtn)
    {
        /*Enable Root Button*/
        lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    }

    /*Add callback*/
    lv_obj_add_event_cb(menu, eventCallBack, LV_EVENT_CLICKED, (void *)menu);

    return menu;
}

/**
 * @brief Collapse the Drop Down List whenever the screen is tapped
 * 
 * @param gui_element GUI_t member of the main ClockAlarmUI_t object, i.e. gui_inst
 */
void collapseDropDownList(GUI_t *gui_element)
{
    /*Check if theres an active drop down list, if so then delete it*/
    if( gui_element->dropDownList != NULL)
    {
        /*Delte the list object*/
        lv_obj_del(gui_element->dropDownList);

        /*Set the element to NULL*/
        gui_element->dropDownList = NULL;
    }
}

/*EVENT HANDLERS--------------------------------------------------------------------------------------------------------------------------------*/

void eventHandlerAlarmSave( lv_event_t *event )
{
    guiEvent_t alarmSaveEvent;
    lv_event_code_t code = lv_event_get_code( event );
    if( code == LV_EVENT_CLICKED )
    {
        alarmSaveEvent.signal = E_SETTING_ALARM_SAVE;
        clockAlarmUIProcessEvent( &clockAlarmUI_inst, &alarmSaveEvent );
    }
}

void eventHandlerThemeSwitch( lv_event_t *event )
{
    /*Create Event*/
    guiTimeChangeEvent_t themeSwitch;
    themeSwitch.mainEvent.signal = E_THEME_SWITCH;

    /*Process Event*/
    clockAlarmUIProcessEvent( &clockAlarmUI_inst, &themeSwitch.mainEvent );
}

/**
 * @brief Event Handler for when user clicks the 'Close' button on the alarm notif page
 * 
 * @param event 
 */
void eventHandlerNotifClose( lv_event_t *event )
{
    /*Create Empty event and get code*/
    guiEvent_t alarmNotifCloseBtnEvent;
    lv_event_code_t eventCode = lv_event_get_code( event );

    /*Get Target, in this case we know its the 'Close' Button on the alarm notif page*/
    lv_obj_t *closeBtn = lv_event_get_target( event );

    if( eventCode == LV_EVENT_CLICKED )
    {
        /*If correct event code then set up signal and process it*/
        alarmNotifCloseBtnEvent.signal = E_ALARM_NOTIF_CLOSE;
        clockAlarmUIProcessEvent( &clockAlarmUI_inst, &alarmNotifCloseBtnEvent );
    }
}

/**
 * @brief Event Handler for when a button is pressed on the 'Save Settings?' Message Box
 * 
 * @param event 
 */
void eventHandlerMessageBox( lv_event_t *event )
{
    /*Get code*/
    lv_event_code_t eventCode = lv_event_get_code( event );

    /*Get target, in this case we know its the message box*/
    lv_obj_t *messageBox = lv_event_get_current_target( event );

    /*Create a Generic event*/
    guiGenericEvent_t messageBoxEvent;

    if( eventCode == LV_EVENT_VALUE_CHANGED )
    {
        /*Get the active Button*/
        usrSaveSettingSelection activeBtnId = lv_msgbox_get_active_btn( messageBox );

        /*Close the MessageBox*/
        lv_msgbox_close( messageBox );

        /*Set up the event*/
        messageBoxEvent.mainEvent.signal = E_SETTING_SAVE_YES_NO;
        messageBoxEvent.param = activeBtnId;

        /*Process Event*/
        clockAlarmUIProcessEvent( &clockAlarmUI_inst, &messageBoxEvent.mainEvent );
    }
}

/**
 * @brief Event Handler for when the Display slider changes value
 * 
 * @param event 
 */
void eventHandlerDisplaySlider( lv_event_t *event )
{
    /*Get Event code*/
    lv_event_code_t eventCode = lv_event_get_code( event );

    /*Get target, in this case we know its a slider*/
    lv_obj_t *displaySlider = ( lv_obj_t * )lv_event_get_target( event );

    /*Get Label, stored in the user data*/
    lv_obj_t *displayLabel = ( lv_obj_t * )lv_event_get_user_data( event );

    /*Update label according the value of slider*/
    lv_label_set_text_fmt( displayLabel, "%d%%", lv_slider_get_value( displaySlider ) );
}

/**
 * @brief Event Handler for when the Alarm ON/OFF switch changes value
 * 
 * @param event 
 */
void eventHandlerAlarmONOFFSwitch(lv_event_t *event)
{
    /*Create Time change event*/
    guiTimeChangeEvent_t timeChangeEventAlarmSwitch;

    /*Get event code*/
    lv_event_code_t eventCode = lv_event_get_code( event );

    /*Get target*/
    lv_obj_t *alarmONOFFSwitch = lv_event_get_target( event );

    /*Get current state of Alarm ON OFF Switch*/
    bool alarmSwitchState = lv_obj_has_state(alarmONOFFSwitch, LV_STATE_CHECKED);

    /*Set signal of the created Time Change event*/
    timeChangeEventAlarmSwitch.mainEvent.signal = E_SETTING_ALARM_ON_OFF;

    /*Get label of the Switch*/
    lv_obj_t *ONOFFSwitchLabel = lv_event_get_user_data( event );

    if( alarmSwitchState )
    {
        /*If alarm switch state = 1, means its on , means user activated Alarm*/

        /*Set text and event param accordingly*/
        lv_label_set_text( ONOFFSwitchLabel, "Alarm : ON");
        timeChangeEventAlarmSwitch.param = ALARM_ON;
    }
    else
    {
        /*Switch is off, set text accordingly*/
        lv_label_set_text( ONOFFSwitchLabel, "Alarm : OFF");
        timeChangeEventAlarmSwitch.param = ALARM_OFF;
    }

    /*Process the event*/
    clockAlarmUIProcessEvent( &clockAlarmUI_inst, &timeChangeEventAlarmSwitch.mainEvent );
}

/**
 * @brief Event Handler for when any of the Alarm Rollers value is changed
 * 
 * @param event z6
 */
void eventHandlerAlarmRollers(lv_event_t *event)
{
    event_t sig;
    uint8_t rollerVal;
    guiTimeChangeEvent_t timeChangeEvent;

    /*Get event code*/
    lv_event_code_t eventCode = lv_event_get_code(event);

    /*Get target*/
    lv_obj_t *target = lv_event_get_target(event);

    /*Get User data, in this case its the roller, (hour or minute)*/
    rollerData_t *rollerData = ( rollerData_t *)lv_event_get_user_data( event );

    if( eventCode == LV_EVENT_VALUE_CHANGED )
    {
        char str[3];

        /*Get the current value*/
        lv_roller_get_selected_str( target, str, sizeof( str ) );

        /*Convert string to value*/
        rollerVal = atoi( str );

        /*Check which roller caused the event (hour or min)*/
        if( rollerData->rollerType == ROLLER_HOUR )
        {
            /*Set the appropriate signal*/
            sig = E_SETTING_ALARM_HOUR;
        }
        else if( rollerData->rollerType == ROLLER_MIN )
        {
            /*Set the appropriate signal*/
            sig = E_SETTING_ALARM_MIN;
        }
        else
        {
            sig = E_NONE;
        }

        /*Add the signal to the time change event*/
        timeChangeEvent.mainEvent.signal = sig;

        /*Add current roller value as a param to the time change event*/
        timeChangeEvent.param = rollerVal;

        /*Process the event*/
        clockAlarmUIProcessEvent( &clockAlarmUI_inst, &timeChangeEvent.mainEvent );
    }

}

/**
 * @brief Event Handler for when the user clicks the 'Save' button on the Time Page
 * 
 * @param event 
 */
void eventHandlerTimeSave(lv_event_t *event)
{
    /*Create Event*/
    guiEvent_t timeSaveEvent;

    /*Get event code*/
    lv_event_code_t eventCode = lv_event_get_code(event);

    /*Check if code matches*/
    if( eventCode == LV_EVENT_CLICKED )
    {
        /*Set event signal*/
        timeSaveEvent.signal = E_SETTING_CLOCK_SAVE;

        /*Process Event*/
        clockAlarmUIProcessEvent(&clockAlarmUI_inst, &timeSaveEvent);
    }
}

/**
 * @brief Event Handler for when the 24Hour switch changes value
 * 
 * @param event 
 */
void eventHandler12H24HSwitch(lv_event_t *event)
{
    /*Create Time Change Event*/
    guiTimeChangeEvent_t timeChangeEventswitch12H24H;

    /*Get event Code*/
    lv_event_code_t eventCode = lv_event_get_code(event);

    /*Get Target, in this case its the switch*/
    lv_obj_t *switch12H24H = lv_event_get_target(event);

    /*Get current state of Meridiem Switch*/
    bool meridiemSwitchState = lv_obj_has_state( switch12H24H, LV_STATE_CHECKED );

    /*Set signal of created Time Change Event*/
    timeChangeEventswitch12H24H.mainEvent.signal = E_SETTING_CLOCK_12H_24H;

    if( meridiemSwitchState )
    {
        /*If meridiemSwitchState = 1, means its on, means user wants 24H*/
        timeChangeEventswitch12H24H.param = MODE_24H;
    }
    else
    {
        /*Switch is off, means 12H*/
        timeChangeEventswitch12H24H.param = MODE_12H;
    }

    /*Process Event*/
    clockAlarmUIProcessEvent(&clockAlarmUI_inst, &timeChangeEventswitch12H24H.mainEvent);

}

/**
 * @brief Event Handler for the container containing the CheckBoxes for CLOCK and ALARM format, the boxes will be
 *  bubbled so when the boxes trigger an event it causes the parent container to cause an event.
 * 
 * @param event 
 */
void eventHandlerMeridiemCheckBoxes(lv_event_t *event)
{
    /*Create Time Change Event*/
    guiTimeChangeEvent_t timeChangeEventCheckBoxes;

    /*Get User data (radio btn data)*/
    CheckBoxData_t *userDataCheckBox = (CheckBoxData_t *)lv_event_get_user_data(event);

    /*Get the current active Button index, 0 = AM, 1 = PM*/
    uint32_t *prevCheckBoxIndex = &userDataCheckBox->CheckBoxBoxIndex;

    /*Get Container, since we add callback to Container, and the radio btn is bubbled, the event came
    from Container, but it originated from radio btn*/
    lv_obj_t *container = lv_event_get_current_target(event);

    /*Get the box which caused the event originally*/
    lv_obj_t *newCheckBox = lv_event_get_target(event);
    uint32_t newCheckBoxIndex = lv_obj_get_index(newCheckBox);

    /*Get the previously checked box, stored in CheckBoxIndex*/
    lv_obj_t *prevCheckBox = lv_obj_get_child(container, *prevCheckBoxIndex);


    if( newCheckBox == container )
    {
        /*If the original event came from container, i.e. user clicked on container then do nothing*/

        return;
    }
    else
    {
        /*User clicked either AM or PM Box at this point*/

        /*Remove state from the previously and currently selected radio btn*/
        lv_obj_clear_state(prevCheckBox, LV_STATE_CHECKED);
        lv_obj_add_state( newCheckBox, LV_STATE_CHECKED );

        /*Add state to newly selected radio btn*/
        lv_obj_add_state(newCheckBox, LV_STATE_CHECKED);

        /*Check weather the CheckBox Event Came from Time or Alarm*/
        if( userDataCheckBox->CheckBoxSettingType == SETTING_TIME )
        {
            /*Event came from Clock Format*/
            timeChangeEventCheckBoxes.mainEvent.signal = E_SETTING_CLOCK_MERIDIEM;
        }
        else if( userDataCheckBox->CheckBoxSettingType == SETTING_ALARM )
        {
            /*Event came from Alarm Format*/
            timeChangeEventCheckBoxes.mainEvent.signal = E_SETTING_ALARM_MERIDIEM;
        }
        else
        {
            timeChangeEventCheckBoxes.mainEvent.signal = E_NONE;
        }

        /*Update previous checkbox, since now the current checkbox is the previous checkbox for next ISR*/
        userDataCheckBox->CheckBoxBoxIndex = newCheckBoxIndex;

        /*Store the Index of the newly selected as paramter to the event*/
        timeChangeEventCheckBoxes.param = newCheckBoxIndex;

        /*Process the event*/
        clockAlarmUIProcessEvent(&clockAlarmUI_inst, &timeChangeEventCheckBoxes.mainEvent);
    }
}


/**
 * @brief Event Handler for when any of the Time rollers value is changed
 * 
 * @param event lv_event_t event
 */
void eventHandlerTimeRollers(lv_event_t *event)
{
    event_t sig;
    uint8_t rollerVal;
    guiTimeChangeEvent_t timeChangeEvent;

    /*Get event code*/
    lv_event_code_t code = lv_event_get_code(event);

    /*Get Target*/
    lv_obj_t *target = lv_event_get_target(event);

    /*Get User data, in this case its the roller (hour or minute or second)*/
    rollerData_t *rollerData = (rollerData_t *)lv_event_get_user_data(event);

    if( code == LV_EVENT_VALUE_CHANGED )
    {
        char str[3];

        /*Get the current value*/
        lv_roller_get_selected_str(target, str, sizeof(str));

        /*Convert string to value*/
        rollerVal = atoi(str);

        /*Check which roller caused the event (hour or min or sec)*/
        if( rollerData->rollerType == ROLLER_HOUR )
        {
            /*Set the appropriate signal*/
            sig = E_SETTING_CLOCK_HOUR;
        }
        else if( rollerData->rollerType == ROLLER_MIN )
        {
            /*Set appropriate signal*/
            sig = E_SETTING_CLOCK_MIN;
        }
        else if( rollerData->rollerType == ROLLER_SEC )
        {
            /*Set appropriate signal*/
            sig = E_SETTING_CLOCK_SEC;
        }
        else
        {
            sig = E_NONE;
        }

        /*Add signal to the time change event*/
        timeChangeEvent.mainEvent.signal = sig;

        /*Add current roller value as a param to the time change event*/
        timeChangeEvent.param = rollerVal;

        /*Process the event*/
        clockAlarmUIProcessEvent(&clockAlarmUI_inst, &timeChangeEvent.mainEvent);
    }
}

/**
 * @brief Event Handler for when the 'Save Button' is clicked on the Date Page
 * 
 * @param event lv_event_t event
 */
void eventHandlerDateSave(lv_event_t *event)
{
    /*Create Event*/
    guiEvent_t dateSaveEvent;

    /*Get Code*/
    lv_event_code_t code = lv_event_get_code(event);

    if( code == LV_EVENT_CLICKED )
    {
        /*If button was clicked*/

        /*Set event signal*/
        dateSaveEvent.signal = E_SETTING_DATE_SAVE;

        /*Process event*/
        clockAlarmUIProcessEvent(&clockAlarmUI_inst, &dateSaveEvent);
    }
}

/**
 * @brief Event Handler for whenever the Date Changes
 * 
 * @param event lv_event_t event
 */
void eventHandlerDateChange(lv_event_t *event)
{
    /*Get event code*/
    lv_event_code_t eventCode = lv_event_get_code(event);

    /*Get target, in form of lv_obj_t * and lv_calendar_t * */
    lv_obj_t *calendarObject = lv_event_get_current_target(event);
    lv_obj_t *btnMatrix = lv_calendar_get_btnmatrix(calendarObject);

    /*Place holders for date and Date Change Event*/
    lv_calendar_date_t date;
    guiDateChangeEvent_t dateChangeEvent;

    if( eventCode == LV_EVENT_VALUE_CHANGED )
    {
        /*If the event was caused by a value being changed*/

        if( lv_calendar_get_pressed_date( calendarObject, &date ) )
        {
            /*If the pressed date is valid*/

            /*Get selected button*/
            int currBtn = lv_btnmatrix_get_selected_btn(btnMatrix);
            
            /*Clear the previous selection highlight for all buttons*/
            lv_btnmatrix_clear_btn_ctrl_all(btnMatrix, LV_BTNMATRIX_CTRL_CUSTOM_2);

            /*Highlight current selected Day*/
            lv_btnmatrix_set_btn_ctrl(btnMatrix, currBtn, LV_BTNMATRIX_CTRL_CUSTOM_2);

            /*Set event signal to send to main event processing function*/
            dateChangeEvent.mainEvent.signal = E_SETTING_DATE_CHANGE;

            /*Set event data*/
            dateChangeEvent.date = date.day;
            dateChangeEvent.month = date.month;
            dateChangeEvent.year = date.year;

            /*Send event to main event processing function*/
            clockAlarmUIProcessEvent(&clockAlarmUI_inst, &dateChangeEvent.mainEvent);
        }
    }
}

/**
 * @brief Event Handler when Root Back Button on Settings Page has been pressed
 * 
 * @param event lv_event_t event
 */
void eventHandlerRootBackBtn(lv_event_t *event)
{
    /*Get Target*/
    lv_obj_t *target = lv_event_get_target(event);

    /*Get User data, in this case the pointer to menu object*/
    lv_obj_t *menu = lv_event_get_user_data(event);

    /*Check if target object is rootback button of menu*/
    if( lv_menu_back_btn_is_root(menu, target))
    {
        /*Create an Event and Process it*/
        guiEvent_t event;
        event.signal = E_SETTING_ROOT_BACK;
        clockAlarmUIProcessEvent(&clockAlarmUI_inst, &event);
    }
}

/**
 * @brief Event Handler for when the screen has been pressed
 * 
 * @param e lv_event_t event
 */
void eventHandlerScreen( lv_event_t * e )
{
    /*Get the event code, for ex, LV_EVENT_CLICKED*/
    lv_event_code_t code = lv_event_get_code(e);
    
    /*Get user data*/
    GUI_t *const gui_element = ( GUI_t *const )lv_event_get_user_data(e);

    /*If the screen is pressed then collapse the dropdown list*/
    if( code == LV_EVENT_PRESSED )
    {
        collapseDropDownList(gui_element);
    }
    else
    {
        LV_LOG_USER("Nothing to collapse\n");
    }
}

/**
 * @brief Event Handler for when the 'Settings' option is selected in the list
 * 
 * @param e lv_event_t event
 */
void eventHandlerDropDownBtnOptionSettings(lv_event_t *e)
{
    /*Get event code*/
    lv_event_code_t code = lv_event_get_code(e);

    /*If code is LV_EVENT_CLICKED then call clockAlarmUIProcessEvent*/
    if( code == LV_EVENT_CLICKED )
    {
        /*Create an event and set its signal to E_SETTING*/
        guiEvent_t event;
        event.signal = E_SETTING; 
        
        /*Process the event of opening the menu after clicking the 'Settings' option from list*/
        clockAlarmUIProcessEvent(&clockAlarmUI_inst, &event);
    }
}

/**
 * @brief Event Handler for when the 'About' option is selected in the list
 * 
 * @param e lv_event_t event
 */
void eventHandlerDropDownBtnOptionAbout(lv_event_t* e)
{
    /*Get event code*/
    lv_event_code_t code = lv_event_get_code(e);
    
    /*Get user data*/
    GUI_t *const gui_element = (GUI_t *const)lv_event_get_user_data(e);
    
    /*Delete dropdown list*/
    collapseDropDownList(gui_element);
    
    if (code == LV_EVENT_CLICKED) 
    {
        /*Create a msgbox with the 'about' info on it*/
        lv_obj_t* mbox1 = lv_msgbox_create(NULL, "About", "Clock version: 1.0\nBy Zafeer Abbasi\nPowered by LVGL v8.3", NULL, true);
        
        /*Center the msgbox*/
        lv_obj_center(mbox1);
    }
}

/**
 * @brief Create a new list everytime the drop down btn is pressed
 * 
 * @param e lv_event_t event
 */
void eventHandlerDropDownBtn(lv_event_t *e)
{
    /*Get event code*/
    lv_event_code_t code = lv_event_get_code(e);

    /*Get user data*/
    GUI_t *const gui_element = (GUI_t *const)lv_event_get_user_data(e);

    /*Create a btn place holder*/
    lv_obj_t *btn;

    /*If the drop down button is pressed, then create a drop down button*/
    if( code == LV_EVENT_CLICKED )
    {
        /*Create List, dropdown doesnt work since its too hard to implement a callback per list option*/
        gui_element->dropDownList = lv_list_create(gui_element->screen);

        if( gui_element->theme == THEME_DARK )
        {
            lv_obj_set_style_bg_color( gui_element->dropDownList, gui_element->darkThemeColor, LV_PART_MAIN );
        }

        /*Create a list button and store the lv_obj_t pointer in the btn var so we can add a cb to it's address*/
        btn = lv_list_add_btn(gui_element->dropDownList, LV_SYMBOL_SETTINGS, "Settings");

        if( gui_element->theme == THEME_DARK )
        {
            lv_obj_set_style_bg_color( btn, gui_element->darkThemeColor, LV_PART_MAIN );
            lv_obj_set_style_text_color( btn, lv_color_white(), LV_PART_MAIN );
        }

        /*Add a cb to the btn pointer which contains the address of the 'Settings' button*/
        lv_obj_add_event_cb(btn, eventHandlerDropDownBtnOptionSettings, LV_EVENT_CLICKED, (void *)gui_element);

        /*Create a list button - 'About' and store the address in the temp btn var*/
        btn = lv_list_add_btn(gui_element->dropDownList, LV_SYMBOL_FILE, "About");

        if( gui_element->theme == THEME_DARK )
        {
            lv_obj_set_style_bg_color( btn, gui_element->darkThemeColor, LV_PART_MAIN );
            lv_obj_set_style_text_color( btn, lv_color_white(), LV_PART_MAIN );
        }
        
        /*Add a cb to the btn pointer which contains address of the 'About' button*/
        lv_obj_add_event_cb(btn, eventHandlerDropDownBtnOptionAbout, LV_EVENT_CLICKED, (void *)gui_element);

        /*Set drop down list height*/
        lv_obj_set_height(gui_element->dropDownList, LV_SIZE_CONTENT);

        /*Align drop down list*/
        lv_obj_align(gui_element->dropDownList, LV_ALIGN_TOP_RIGHT, 0, 0);
    }
    
}
