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
#if(MCU_CORE_B91)
#define LED1            		GPIO_PB4
#define LED2            		GPIO_PB5
#define LED3            		GPIO_PB6
#define LED4            		GPIO_PB7
#define TIMER_GPIO_1    		GPIO_PA0
#define TIMER_GPIO_2     		GPIO_PA1

#elif(MCU_CORE_B92)
#define LED1            		GPIO_PD0
#define LED2            		GPIO_PD1
#define LED3            		GPIO_PE6
#define LED4            		GPIO_PE7
#endif


#define TIMER_SYS_CLOCK_MODE 	1
#define TIMER_GPIO_TRIGGER_MODE 2
#define TIMER_GPIO_WIDTH_MODE 	3
#define TIMER_TICK_MODE 		4
#define TIMER_WATCHDOG_MODE 	5
#if(MCU_CORE_B92)
#define TIMER_32K_WATCHDOG_MODE 6
#endif

#define TIMER_MODE				TIMER_32K_WATCHDOG_MODE


#define WATCHDOG_32K_XTAL_MODE	1
#define WATCHDOG_32K_RC_MODE	2

#define WATCHDOG_MODE			WATCHDOG_32K_RC_MODE















/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
