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



#define STIMER_DELAY				1
#if (MCU_CORE_B91)||(MCU_CORE_B92)
#define STIMER_IRQ					2
#elif (MCU_CORE_B93)
#define STIMER_IRQ_D25				3
#define STIMER_IRQ_D25_N22_DSP		4
#endif
#define STIMER_GET_32K_TICK			5
#define STIMER_SET_32K_TICK			6
#define STIMER_TRACK_32K_TICK		7
#define STIMER_GET_TICK				8
#define STIMER_MODE					STIMER_DELAY




/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
