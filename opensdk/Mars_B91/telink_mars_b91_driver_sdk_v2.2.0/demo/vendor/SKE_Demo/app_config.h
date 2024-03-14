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

#define	 SKE_TEST							1
#define	 SKE_CCM_TEST						2
#define	 SKE_GCM_TEST						3
#define	 SKE_SAMPLE_TEST					4

#define	 SKE_TEST_MODE					SKE_TEST

#define LED1            GPIO_PD0
#define LED2            GPIO_PD1
#define LED3            GPIO_PD2
#define LED4            GPIO_PD3
#define LED5            GPIO_PD4
#define LED6            GPIO_PD5


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
