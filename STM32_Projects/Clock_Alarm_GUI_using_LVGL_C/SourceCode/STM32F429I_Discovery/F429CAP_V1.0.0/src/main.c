/*
# ##############################################################################
# File: main.c                                                                 #
# Project: src                                                                 #
# Created Date: Friday, September 22nd 2023, 6:47:24 pm                        #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Friday, September 22nd 2023, 8:06:41 pm                       #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
 */


#include "lvgl.h"
#include "app_hal.h"
#include "rgb_mixer.h"
#include "demos/lv_demos.h"
#include <stdio.h>
#include <stdlib.h>
#include "examples/lv_examples.h"
#include "project_clock_alarm.h"

/*Main Clock Alarm UI Application object*/
ClockAlarmUI_t clockAlarmUI_inst;

int main(void)
{
  lv_init();
  
  hal_setup();

  lv_disp_set_rotation(lv_disp_get_default(),LV_DISP_ROT_90);
  
  clockAlarmUIConstructor(&clockAlarmUI_inst);

	hal_loop();
}
