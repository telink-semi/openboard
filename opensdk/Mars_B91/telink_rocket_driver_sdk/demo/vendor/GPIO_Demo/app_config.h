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
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#include "driver.h"
#include "common.h"

#define NORMAL_MODE 			1
#define MANUAL_TEST_MODE 		2 // For internal testing, users need not care
#define AUTO_TEST_MODE 			3 // For internal testing, users need not care
#define	DEMO_MODE				NORMAL_MODE

#if(DEMO_MODE == NORMAL_MODE)
#define GPIO_IRQ				1
#define GPIO_IRQ_RISC0			2
#define GPIO_IRQ_RISC1			3
#define GPIO_HIGH_RESISTOR		4
#if(MCU_CORE_B92||MCU_CORE_B93||MCU_CORE_B95)
#define GPIO_SEL_IRQ_SRC        5
#endif
#define GPIO_MODE 				GPIO_HIGH_RESISTOR
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
