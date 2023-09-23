/*
# ##############################################################################
# File: tft.h                                                                  #
# Project: stm32f429_disco                                                     #
# Created Date: Wednesday, August 23rd 2023, 2:24:52 am                        #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Wednesday, August 23rd 2023, 2:56:04 am                       #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
*//**/

/**
 * @file disp.h
 * 
 */

#ifndef DISP_H
#define DISP_H

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define TFT_HOR_RES 240
#define TFT_VER_RES 320

#define TFT_EXT_FB		1		/*Frame buffer is located into an external SDRAM*/
#define TFT_USE_GPU		0		/*Enable hardware accelerator*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void tft_init(void);

/**********************
 *      MACROS
 **********************/

#endif
