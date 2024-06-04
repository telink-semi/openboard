/********************************************************************************************************
 * @file    common.h
 *
 * @brief   This is the header file for B91m
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "auto_test/dut_cmd.h"
#include "auto_test/pc_interface.h"
#include "calibration.h"
#include "printf.h"
#include <string.h>

/**
	===============================================================================
						 ##### LED and KEY #####
	===============================================================================
	The default configuration of LEDs and KEYs match the following hardware revisions:
	B91 C1T213A20_V1_3_20200727
	B92 C1T266A20_V1_3_20220722

*/
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
#else
#define LED1           		 	GPIO_PD0
#define LED2            		GPIO_PD1
#define LED3            		GPIO_PD2
#define LED4            		GPIO_PD3
#define LED5            		GPIO_PD4
#define LED6            		GPIO_PD5
#endif
/**
	===============================================================================
						 ##### platform init and clock init #####
	===============================================================================

*/
#if(MCU_CORE_B91)
void platform_init(power_mode_e power_mode, vbat_type_e vbat_v);
#ifndef PLATFORM_INIT
#define PLATFORM_INIT   platform_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6)
#endif
#ifndef CLOCK_INIT
#define CLOCK_INIT		CCLK_24M_HCLK_24M_PCLK_24M
#endif
#elif(MCU_CORE_B92)
void platform_init(power_mode_e power_mode, vbat_type_e vbat_v, gpio_voltage_e gpio_v);
#ifndef PLATFORM_INIT
#define PLATFORM_INIT 	platform_init(LDO_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3)
#endif
#ifndef CLOCK_INIT
#define CLOCK_INIT		CCLK_24M_HCLK_24M_PCLK_24M
#endif
#else
void platform_init(void);
#ifndef PLATFORM_INIT
#define PLATFORM_INIT 	platform_init()
#endif
#ifndef CLOCK_INIT
#define CLOCK_INIT
#endif
#endif


