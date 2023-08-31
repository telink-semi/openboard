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
#elif(MCU_CORE_B92)
#define LED1            		GPIO_PE6
#define LED2            		GPIO_PE7
#define LED3            		GPIO_PD0
#define LED4            		GPIO_PD1
#endif

#define WAKEUP_PAD				GPIO_PA0
#define CURRENT_TEST	     	1
#define CRC_OK			     	1
#define	MDEC_MATCH_VALUE		0x02

#if(MCU_CORE_B91)
#define PM_TICK_STIMER			PM_TICK_STIMER_16M
#endif

/* IDLE MODE */
#define IDLE_TIMER_WAKEUP				1

/**
 * @note To enter sleep using COMPARATOR Wake mode, the voltage difference between the input level and the configured wake level
 * 		 needs to be greater than 100mV. If the input level is particularly close to the wake level, the chip will not sleep properly
 * 		 due to the unstable state of the LPC, resulting in a crash.
 */

/* SUSPEND MODE */
//[eagle]The A0 version of the suspend execution process is abnormal and the program restarts.
#define SUSPEND_PAD_WAKEUP   			10	//62.1uA
#define SUSPEND_32K_RC_WAKEUP   		11	//63.7uA
#define SUSPEND_32K_XTAL_WAKEUP			12	//62.7uA
#if(MCU_CORE_B91)
#define SUSPEND_MDEC_WAKEUP				13	//65.2uA
#endif
#define SUSPEND_COMPARATOR_WAKEUP      	14	//62.3uA
#define SUSPEND_CORE_USB_WAKEUP      	15	//
//#if(MCU_CORE_B92)
//#define SUSPEND_CORE_GPIO_WAKEUP      	16	//
//#endif

/* DEEP SLEEP MODE */
#define DEEP_PAD_WAKEUP		 			20	//0.7uA
#define DEEP_32K_RC_WAKEUP      		21	//1.3uA
#define DEEP_32K_XTAL_WAKEUP      		22	//1.7uA
#if(MCU_CORE_B91)
#define DEEP_MDEC_WAKEUP      			23	//1.4uA
#endif
#define DEEP_COMPARATOR_WAKEUP      	24	//1.6uA

/* DEEP SLEEP WITH RETENTION MODE */
#define DEEP_RET32K_PAD_WAKEUP     		30	//1.8uA
#define DEEP_RET32K_32K_RC_WAKEUP     	31	//2.4uA
#define DEEP_RET32K_32K_XTAL_WAKEUP     32	//2.8uA
#if(MCU_CORE_B91)
#define DEEP_RET32K_MDEC_WAKEUP      	33	//2.6uA
#endif
#define DEEP_RET32K_COMPARATOR_WAKEUP   34	//2.8uA

#define DEEP_RET64K_PAD_WAKEUP     		40	//2.7uA
#define DEEP_RET64K_32K_RC_WAKEUP     	41	//3.2uA
#define DEEP_RET64K_32K_XTAL_WAKEUP     42	//3.7uA
#if(MCU_CORE_B91)
#define DEEP_RET64K_MDEC_WAKEUP      	43	//3.4uA
#endif
#define DEEP_RET64K_COMPARATOR_WAKEUP   44	//3.7uA

#if(MCU_CORE_B92)
#define DEEP_RET96K_PAD_WAKEUP     		50	//2.7uA
#define DEEP_RET96K_32K_RC_WAKEUP     	51	//3.2uA
#define DEEP_RET96K_32K_XTAL_WAKEUP     52	//3.7uA
#define DEEP_RET96K_COMPARATOR_WAKEUP   53	//3.7uA
#endif

#define PM_MODE			     			DEEP_PAD_WAKEUP


/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

