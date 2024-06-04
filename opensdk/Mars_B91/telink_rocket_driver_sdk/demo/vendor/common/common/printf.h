/********************************************************************************************************
 * @file    printf.h
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
#include "gpio.h"
#include "stimer.h"
#include <stdio.h>
#define DEBUG_MODE	1

#if(DEBUG_MODE==1)

#define  UART_PRINT_DEBUG_ENABLE		0
#define  USB_PRINT_DEBUG_ENABLE		    1

#ifndef   DEBUG_BUS
#define   DEBUG_BUS  	UART_PRINT_DEBUG_ENABLE
#endif
/**
 * @brief      This function serves to printf string.
 * @param[in]  *format  -  format string need to print
 * @param[in]  ...   	-  variable number of data
 * @return     none.
 */
#if (DEBUG_BUS==USB_PRINT_DEBUG_ENABLE)
#define   FIFOTHRESHOLD  	4
#define   BLOCK_MODE   		0
#define   EDPS_DAT       reg_usb_ep8_dat
#define   USBFIFO        reg_usb_ep8_fifo_mode


#elif (DEBUG_BUS==UART_PRINT_DEBUG_ENABLE)

#define  PRINT_BAUD_RATE             		115200//1M baud rate,should Not bigger than 1Mb/s
#define  DEBUG_INFO_TX_PIN           		GPIO_PA0//uart tx pin
#if(MCU_CORE_B91|| MCU_CORE_B92)
#define  TX_PIN_OUTPUT_REG				    reg_gpio_out(DEBUG_INFO_TX_PIN)
#else
#define  TX_PIN_OUTPUT_REG				    reg_gpio_out_set_clear(DEBUG_INFO_TX_PIN)
#endif

#define	 BIT_INTERVAL	(SYSTEM_TIMER_TICK_1S/PRINT_BAUD_RATE)

#endif
#endif

