/*
# ##############################################################################
# File: app_hal.c                                                              #
# Project: sdl2                                                                #
# Created Date: Wednesday, August 30th 2023, 7:15:28 pm                        #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Wednesday, August 30th 2023, 7:19:22 pm                       #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/

#include <unistd.h>
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "display/monitor.h"
#include "indev/mouse.h"
#include "indev/mousewheel.h"
#include "indev/keyboard.h"
#include "sdl/sdl.h"
#include "project_clock_alarm.h"

static uint64_t millis1, millis2;

/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void * data)
{
    (void)data;

    while(1) {
        SDL_Delay(1);   /*Sleep for 5 millisecond*/
        lv_tick_inc(1); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}


void hal_setup(void)
{
    // Workaround for sdl2 `-m32` crash
    // https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1775067/comments/7
    #ifndef WIN32
        setenv("DBUS_FATAL_WARNINGS", "0", 1);
    #endif

    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/

    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf[SDL_HOR_RES * 10];                          /*Declare a buffer for 10 lines*/
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, SDL_HOR_RES * 10);    /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);              /*Basic initialization*/
    disp_drv.flush_cb = sdl_display_flush;    /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;
    //disp_drv.disp_fill = monitor_fill;      /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    //disp_drv.disp_map = monitor_map;        /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    lv_disp_drv_register(&disp_drv);

    /* Add the mouse as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);            /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = sdl_mouse_read;       /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    sdl_init();

    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
     * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
}

void hal_loop(void)
{
    millis1 = SDL_GetTicks64( );
    while(1) 
    {
        SDL_Delay(1);
        lv_task_handler();

        if( SDL_GetTicks64( ) - millis1 >= 1000UL )
        {
            /*One Second has passed*/
            /*Update Current Time on Database*/
            dataBaseUpdateCurrentTime( &clockAlarmUI_inst.clock_inst );

            /*Update Current Time on Display*/
            guiUpdateCurrentTime( &clockAlarmUI_inst );

            /*Check for alarm*/
            if( isAlarm( &clockAlarmUI_inst.clock_inst ) && clockAlarmUI_inst.clock_inst.alarmStatus == ALARM_ON )
            {
                /*Create Event and process it*/
                guiEvent_t alarmEvent;
                alarmEvent.signal = E_ALARM_NOTIF_ON;
                clockAlarmUIProcessEvent( &clockAlarmUI_inst, &alarmEvent );
            }

            /*Update Millis1*/
            millis1 = SDL_GetTicks64( );
        }
    }
}
