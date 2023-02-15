/********************************************************************************************************
 * @file    board_9518_mboard.h
 *
 * @brief   This is the header file for board_9518_dongle
 *
 * @author  Zigbee Group
 * @date    2022
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define	BUTTON1 					GPIO_PD2
#define	BUTTON2 					GPIO_PD1
#define	BUTTON3 					GPIO_PC2

#if defined COLOR_RGB_SUPPORT && (COLOR_RGB_SUPPORT == 1)
	#error "To do!"
#else
//PWM configuration, LED_R as warm light, LED_B1 as cool light.
#define LED_R						GPIO_PD3
#define LED_B						GPIO_PB5
#define LED_PERMIT					GPIO_PB6

#define PWM_R_CHANNEL				3	//PWM3_N
#define PWM_R_CHANNEL_SET()			do{\
										pwm_set_pin(LED_R); \
									}while(0)

#define PWM_B_CHANNEL				1	//PWM1_O
#define PWM_B_CHANNEL_SET()			do{\
										pwm_set_pin(LED_B); \
									}while(0)

#define WARM_LIGHT_PWM_CHANNEL		PWM_R_CHANNEL
#define COOL_LIGHT_PWM_CHANNEL		PWM_B_CHANNEL
#define WARM_LIGHT_PWM_SET()		PWM_R_CHANNEL_SET()
#define COOL_LIGHT_PWM_SET()		PWM_B_CHANNEL_SET()

#endif

// UART
#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC2//print
#endif


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
