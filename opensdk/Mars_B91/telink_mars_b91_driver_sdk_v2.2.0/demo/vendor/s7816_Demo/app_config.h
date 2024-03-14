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

#if(MCU_CORE_B92)
#define LED1            GPIO_PD0
#define LED2            GPIO_PD1
#define LED3            GPIO_PE6
#define LED4            GPIO_PE7
#elif(MCU_CORE_B91)
#define LED1            GPIO_PB4
#define LED2            GPIO_PB5
#define LED3            GPIO_PB6
#define LED4            GPIO_PB7
#endif

#define S7816_UART0    0
#define S7816_UART1    1
#define S7816_UART_CHN      S7816_UART0


#if(MCU_CORE_B91)
#define S7816_TRX_PIN    S7816_UART1_RTX_E2
#define S7816_VCC_PIN    GPIO_PE0
#define S7816_RST_PIN    GPIO_PB1
#define S7816_CLK_PIN    S7817_CLK_PA0
#elif(MCU_CORE_B92)
#define S7816_TRX_PIN    GPIO_FC_PB3
#define S7816_VCC_PIN    GPIO_PB4
#define S7816_RST_PIN    GPIO_PB1
#define S7816_CLK_PIN    GPIO_FC_PA0
#endif

#define F   372      //clock frequency regulator ,372 is the initialized  value.
#define D    1       //bitrate regulator,1 is the initialized value.

#define S7816_RX_BUFF_LEN    48


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
