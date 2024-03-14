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
/*
 * Button matrix table:
 * 			KEY3	KEY4
 * 	KEY1	SW2		SW3
 * 	KEY2	SW4		SW5
 */
#define KEY1           			GPIO_PC2
#define KEY2           			GPIO_PC0
#define KEY3           			GPIO_PC3
#define KEY4           			GPIO_PC1
#elif(MCU_CORE_B92)
#define LED3                    GPIO_PD0
#define LED4                    GPIO_PD1
#define LED1                    GPIO_PE6
#define LED2                    GPIO_PE7
/*
 * Button matrix table:
 * 			KEY3	KEY4
 * 	KEY1	SW2		SW3
 * 	KEY2	SW4		SW5
 */
#define KEY1           			GPIO_PE2
#define KEY2           			GPIO_PE3
#define KEY3           			GPIO_PE4
#define KEY4           			GPIO_PF0
#endif

#define GPIO_IRQ				1
#define GPIO_IRQ_RISC0			2
#define GPIO_IRQ_RISC1			3
#define GPIO_HIGH_RESISTOR		4
#if(MCU_CORE_B92)
#define GPIO_SEL_IRQ_SRC        5
#endif
#define GPIO_MODE 				GPIO_IRQ

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
