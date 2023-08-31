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
#define LED1            GPIO_PB4
#define LED2            GPIO_PB5
#define LED3            GPIO_PB6
#define LED4            GPIO_PB7
#elif(MCU_CORE_B92)
#define LED1            GPIO_PD0
#define LED2            GPIO_PD1
#define LED3            GPIO_PE6
#define LED4            GPIO_PE7
#endif

#define INTERRUPT_NESTED_DEMO				1 // external interrupt
#if(MCU_CORE_B91)
#define HSP_DEMO				            2//Hardware Stack Protection
#define WFI_DEMO				            3//(Wait-For-Interrupt Mode)
#endif

#if (MCU_CORE_B92)
#define INTERRUPT_MTIME_DEMO                4 // machine time interrupt
#define INTERRUPT_SWI_DEMO                  5 // software interrupt
#define EXT_MTIME_SWI_NESTED_DEMO           6 // external, mtime and swi interrupt
#endif

#define TRAP_DEMO                       INTERRUPT_NESTED_DEMO















/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
