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
/**
   @verbatim
    Telink SoC integrates ISO-7816 communication module and supports communication with contact IC cards. The S7816 is implemented through the UART function of the SoC, so it will occupy the corresponding UART module when used.
    The demo default baud rate configuration:
    =========================================
     - 10752 baud rate
     - 8 data bits
     - Even parity
     - One stop bit

    The pin function explain:
    =========================================
    In actual use, we need to connect each contact of IC card to SoC one by one, which is the hardware basis for communication between SoC and IC card.
     - VCC is the power supply voltage of IC card. We can choose any one of the free GPIO pins of the SoC to connect to it.
     - RST is the reset signal of IC card. We can choose any one of the free GPIO pins of the SoC to connect to it.
     - CLK is the clock contact of the IC card. The clock is supplied to the IC card by the SoC.
     - TRX, I/O contact, is IC card input and output contact. Because IC card only supports half-duplex communication, at a certain moment I/O contact only supports input or output, so in actual use we need to pay attention to the timing.

    The pin voltages explain:
    =========================================
    The IS07816-3 protocol specifies three operating voltages for IC cards: Class A-5V, Class B-3V, and Class C-1.8V. In actual use, the voltage provided by the SoC needs to match the operating voltage of the card.

   @endverbatim
 */







#pragma once
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#include "driver.h"
#include "common.h"


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
