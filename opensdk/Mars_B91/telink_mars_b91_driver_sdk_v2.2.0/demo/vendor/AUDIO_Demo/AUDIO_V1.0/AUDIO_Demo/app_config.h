/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for B91m
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define LED1            GPIO_PB4
#define LED2            GPIO_PB5
#define LED3            GPIO_PB6
#define LED4            GPIO_PB7







#define LINEIN_TO_LINEOUT               1
#define AMIC_TO_LINEOUT                 2
#define DMIC_TO_LINEOUT                 3
#define BUFFER_TO_LINEOUT               4
#define EXT_CODEC_LINEIN_LINEOUT        5
#define FLASH_TO_LINEOUT                6


#define AUDIO_MODE     DMIC_TO_LINEOUT

#define CHIP_VER_A0              0
#define CHIP_VER_A1              1
#define CHIP_VER             CHIP_VER_A1








/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
