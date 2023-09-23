/*
# ##############################################################################
# File: rgb_mixer.c                                                            #
# Project: src                                                                 #
# Created Date: Friday, August 18th 2023, 7:35:32 pm                           #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Sunday, August 20th 2023, 1:28:32 am                          #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*/


#include "lvgl.h"
#include "app_hal.h"
#include "demos/lv_demos.h"
#include "rgb_mixer.h"

lv_obj_t *rect;

enum{
    
    SLIDER_R = 0,
    SLIDER_G,
    SLIDER_B
};

typedef struct{
    
    uint8_t slider_type;
    lv_obj_t *label;
    
}rgb_mixer_label_t;


/*Slider Callback*/
void slider_callback(lv_event_t *e)
{
    static uint8_t r,g,b;

    /*Get info from param: lv_event_t*/
    lv_obj_t *slider = lv_event_get_target(e);
    rgb_mixer_label_t *user_data = lv_event_get_user_data(e);
    int32_t value = lv_slider_get_value(slider);

    /*Update value*/
    lv_label_set_text_fmt(user_data->label, "%d", value);
    
    switch (user_data->slider_type)
    {
        case SLIDER_R:
            r = value;
            break;
            
        case SLIDER_G:
            g = value;
            break;
            
        case SLIDER_B:
            b = value;
            break;    
        
        default:
            break;
    }

    lv_obj_set_style_bg_color(rect, lv_color_make(r, g, b), LV_PART_MAIN);
    
}


void rgb_mixer_create_ui(void)
{
    static rgb_mixer_label_t r,g,b;

    r.slider_type = SLIDER_R;
    g.slider_type = SLIDER_G;
    b.slider_type = SLIDER_B;
    
    /*create slider objects and align them*/
    lv_obj_t *slider_r = lv_slider_create(lv_scr_act());
    lv_obj_t *slider_g = lv_slider_create(lv_scr_act());
    lv_obj_t *slider_b = lv_slider_create(lv_scr_act());

    /*Set value range for sliders*/
    lv_slider_set_range(slider_r, 0, 255);
    lv_slider_set_range(slider_g, 0, 255);
    lv_slider_set_range(slider_b, 0, 255);
    
    lv_obj_align(slider_r, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_align_to(slider_g, slider_r, LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_align_to(slider_b, slider_g, LV_ALIGN_TOP_MID, 0, 50);

    /*create base object for rectangle and set size and align*/
    rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(rect, 300, 60);
    lv_obj_align_to(rect, slider_b, LV_ALIGN_TOP_MID, 0, 30);
    
    /*Appy colours to the sliders for RGB*/
    lv_obj_set_style_bg_color(slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider_r, lv_palette_main(LV_PALETTE_RED), LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider_g, lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider_b, lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB);

    /*Add border to rectangle*/
    lv_obj_set_style_border_color(rect, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(rect, 5, LV_PART_MAIN);

    /*Create Main label*/
    lv_obj_t *heading = lv_label_create(lv_scr_act());
    lv_label_set_text(heading, "RGB Mixer");
    lv_obj_align(heading, LV_ALIGN_TOP_MID, 0, 7);

    /*RGB mixer labels*/
    r.label = lv_label_create(lv_scr_act());
    lv_label_set_text(r.label, "0");
    lv_obj_align_to(r.label, slider_r, LV_ALIGN_TOP_MID, 0, 0);

    g.label = lv_label_create(lv_scr_act());
    lv_label_set_text(g.label, "0");
    lv_obj_align_to(g.label, slider_g, LV_ALIGN_TOP_MID, 0, 0);

    b.label = lv_label_create(lv_scr_act());
    lv_label_set_text(b.label, "0");
    lv_obj_align_to(b.label, slider_b, LV_ALIGN_TOP_MID, 0, 0);

    /*Slider callback implementation*/
    lv_obj_add_event_cb(slider_r, slider_callback, LV_EVENT_VALUE_CHANGED, &r);
    lv_obj_add_event_cb(slider_g, slider_callback, LV_EVENT_VALUE_CHANGED, &g);
    lv_obj_add_event_cb(slider_b, slider_callback, LV_EVENT_VALUE_CHANGED, &b);
}

