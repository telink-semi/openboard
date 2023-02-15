/********************************************************************************************************
 * @file    board_8258_dongle.h
 *
 * @brief   This is the header file for board_8258_dongle
 *
 * @author  Zigbee Group
 * @date    2021
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

//KEY
#define	BUTTON1 					GPIO_PD2
#define	BUTTON2 					GPIO_PD1
#define	BUTTON3 					GPIO_PC2

//LED
#define LED_R						GPIO_PB6
#define LED_PERMIT					LED_R

// UART
#if ZBHCI_UART
	#define UART_TX_PIN         	UART0_TX_PB2
	#define UART_RX_PIN         	UART0_RX_PB3
	#define UART_PIN_CFG()			uart_set_pin(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC2//print
#endif

// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
	#define HW_USB_CFG()			do{ \
										usb_set_pin_en();	\
									}while(0)
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
