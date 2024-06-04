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


#define  GPIO_TOGGLE				1

#define	 RF_BLE_1M					1
#define	 RF_BLE_2M					2
#define	 RF_BLE_1M_NO_PN			3
#define  RF_ZIGBEE_250K				4
#define	 RF_BLE_2M_NO_PN			5
#define  RF_LR_S2_500K	    		6
#define  RF_LR_S8_125K				7
#define  RF_PRIVATE_250K			8
#define  RF_PRIVATE_500K			9
#define  RF_PRIVATE_1M	    		10
#define  RF_PRIVATE_2M				11
#define  RF_ANT						12
#define  RF_HYBEE_1M				13
#define  RF_HYBEE_2M				14
#define  RF_HYBEE_500K				15
#define  RF_BLE_STX2RX   			16
#define RF_BLE_SDK_TEST				20


#if(MCU_CORE_B92)
#define	 RF_USER_DEFINE_PKT 		17
#define	 RF_BLE_1M_HYPER_LENGTH		18
#endif

#define RF_MODE 			RF_BLE_SDK_TEST

#if(MCU_CORE_B91)
#define RF_POWER			RF_POWER_P4p35dBm
#elif(MCU_CORE_B92)
#define RF_POWER			RF_POWER_P5p21dBm
#elif(MCU_CORE_B93)
#define RF_POWER			RF_POWER_P4p35dBm
#endif
















/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
