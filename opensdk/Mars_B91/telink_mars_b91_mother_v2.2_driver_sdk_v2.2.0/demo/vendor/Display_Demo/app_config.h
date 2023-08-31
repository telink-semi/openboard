/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for B91m
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once
#include "driver.h"
#if defined(__cplusplus)
extern "C" {
#endif

#define LED1            GPIO_PD6
#define LED2            GPIO_PD7
#define LED3            GPIO_PE0
#define LED4            GPIO_PE1

#define SPI_LCD  		 	1//support 320*385 ST7796H2 LCD display
#define SPI_OLED 		 	2//support 454*454 RM69330 OLED display
#define SPI_RAMLESS_LCD  	3//support 400*400 and 320*380 ST77903 Ramless LCD display

#define SPI_DISPLAY_SELECT  SPI_RAMLESS_LCD

#define	 RGB565			1
#define	 RGB666			2
#define	 RGB888			3

#define RGB_SELECT  RGB565

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
