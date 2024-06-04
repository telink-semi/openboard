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

#if(MCU_CORE_B91)
#define UART0_RTX_PIN	UART0_RX_PB3
#define UART0_TX_PIN    UART0_TX_PB2
#define UART0_RX_PIN    UART0_RX_PB3
#define UART0_CTS_PIN   UART0_CTS_PA1
#define UART0_RTS_PIN   UART0_RTS_PA2

#define UART1_RTX_PIN	UART1_RX_PC7
#define UART1_TX_PIN    UART1_TX_PC6
#define UART1_RX_PIN    UART1_RX_PC7
#define UART1_CTS_PIN   UART1_CTS_PC4
#define UART1_RTS_PIN   UART1_RTS_PC5




#elif(MCU_CORE_B92)

#define UART0_RTX_PIN 			GPIO_FC_PB3
#define UART0_TX_PIN            GPIO_FC_PB1
#define UART0_RX_PIN            GPIO_FC_PB2
#define UART0_CTS_PIN           GPIO_FC_PB4
#define UART0_RTS_PIN           GPIO_FC_PA0

#define UART1_RTX_PIN 			GPIO_FC_PB3
#define UART1_TX_PIN            GPIO_FC_PB1
#define UART1_RX_PIN            GPIO_FC_PB2
#define UART1_CTS_PIN           GPIO_FC_PB4
#define UART1_RTS_PIN           GPIO_FC_PA0
#elif(MCU_CORE_B93)
#define UART0_RTX_PIN 			UART0_RX_PB2
#define UART0_TX_PIN            UART0_TX_PB1
#define UART0_RX_PIN            UART0_RX_PB2
#define UART0_CTS_PIN           UART0_CTS_PB3
#define UART0_RTS_PIN           UART0_RTS_PB4

#define UART1_RTX_PIN 			UART1_RX_PB2
#define UART1_TX_PIN            UART1_TX_PB1
#define UART1_RX_PIN            UART1_RX_PB2
#define UART1_CTS_PIN           UART1_CTS_PB3
#define UART1_RTS_PIN           UART1_RTS_PB4

#define UART2_RTX_PIN 			UART2_RX_PB2
#define UART2_TX_PIN            UART2_TX_PB1
#define UART2_RX_PIN            UART2_RX_PB2
#define UART2_CTS_PIN           UART2_CTS_PB3
#define UART2_RTS_PIN           UART2_RTS_PB4

#define UART3_RTX_PIN 			UART3_RX_PB2
#define UART3_TX_PIN            UART3_TX_PB1
#define UART3_RX_PIN            UART3_RX_PB2
#define UART3_CTS_PIN           UART3_CTS_PB3
#define UART3_RTS_PIN           UART3_RTS_PB4
#elif(MCU_CORE_B95)
#define UART0_RTX_PIN           GPIO_FC_PC5
#define UART0_TX_PIN            GPIO_FC_PC4
#define UART0_RX_PIN            GPIO_FC_PC5
#define UART0_CTS_PIN           GPIO_FC_PC6
#define UART0_RTS_PIN           GPIO_FC_PC7

#define UART1_RTX_PIN           GPIO_FC_PC5
#define UART1_TX_PIN            GPIO_FC_PC4
#define UART1_RX_PIN            GPIO_FC_PC5
#define UART1_CTS_PIN           GPIO_FC_PC6
#define UART1_RTS_PIN           GPIO_FC_PC7

#define UART2_RTX_PIN           GPIO_FC_PC5
#define UART2_TX_PIN            GPIO_FC_PC4
#define UART2_RX_PIN            GPIO_FC_PC5
#define UART2_CTS_PIN           GPIO_FC_PC6
#define UART2_RTS_PIN           GPIO_FC_PC7
#endif

#define UART_DMA  		    1     //uart use dma
#define UART_NDMA  	   		2     //uart not use dma
#if(MCU_CORE_B92||MCU_CORE_B93||MCU_CORE_B95)
#define UART_DMA_LLP        3
#endif
#define UART_MODE	 	    UART_NDMA

#define BASE_TX	   		0 //just for NDMA
#define NORMAL	   		1
#define USE_CTS    		2
#define USE_RTS    		3
#define FLOW_CTR  		NORMAL


#define UART_1WIRE_MODE       0  //tx and rx is a same line
#define UART_2WIRE_MODE       1  //tx and rx are two lines
#define UART_WIRE_MODE        UART_2WIRE_MODE

/*Previously, the external general feedback was about the abnormal combination of uart and pm,
 *sample code combining Pm is given in no_dma and dma, respectively,things to pay attention to in use are as follows:
 *In no_dma:
 *1.before suspend, wait for uart to complete receiving and sending,
 *after entering suspend, the uart cannot work, resulting in abnormal data sending and receiving.
 *2.after suspend wakes up, the chip go to uart_reset,The hardware read and write pointer will be cleared to zero.
 *Interfaces uart_clr_tx_index and uart_clr_rx_index need to be called to clear the software read and write pointer.
 *Otherwise, when the uart sends or receives data, the software index does not match the hardware pointer, resulting in data exceptions.
 *In dma:
 *1.In Before suspend sleep in dma, you need to wait for uart to send and receive,
 *after entering suspend, the uart cannot work, resulting in abnormal data sending and receiving,
 *2.uart_receive_dma configured after suspend or before sleep is invalid, and you need to call uart_receive_dma again ,
 *if you are receiving data.
*/
#define WAKEUP_PAD				    GPIO_PA0
#define UART_DEMO_WITH_PM_FUNC      1
#define UART_DEMO_WITHOUT_PM_FUNC   0



#define UART_DEMO_COMBINED_WITH_PM_FUNC	  UART_DEMO_WITHOUT_PM_FUNC //












/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
