/********************************************************************************************************
 * @file    puya_flash_trim.h
 *
 * @brief   This is the header file for B91m
 *
 * @author  Driver Group
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
 *
 *******************************************************************************************************/
#ifndef DRIVERS_B91_FLASH_TRIM_H_
#define DRIVERS_B91_FLASH_TRIM_H_
#include "driver.h"
#include "compiler.h"
#include "puya_common.h"
/**
 * @brief		This function serves to raise the flash reset voltage to 2.1V. If flash trim takes effect, the vbat voltage of chip can not be lower than 2.1V.
 * @return		0:OK; 1:fail to trim; 2: fail to enter test mode; 3:The chip mid is not 0x146085.
 * @note        Flash specification states that the default reset voltage of flash is 1.0V.
 *              Important description: The return value of this function is "0", which does not mean that flash trim is successful.Flash trim will take effect
 * 				after flash is powered off and powered on again.
 * 				Risk description:This function needs to use delay function and disable interrupt.The user must determine
 * 				how to use this function and better use it once,otherwise it may cause risks.Dont call this function anymore if flash trim takes effect.
 * 				Pay attention to the threshold of battery voltage detection and do not call this function in a low voltage state.
 * 				Flash cannot be powered off while calling this function.
 */
_attribute_text_sec_ unsigned char flash_trim(void);
/**
 * @brief		This function serves to check whether flash trim is OK.
 * @return		0:OK; 1:fail to trim check; 2: fail to enter test mode; 3:The chip mid is not 0x146085.
 * @note        Risk description: This function needs to use delay function and disable interrupt.The user must determine
 * 				how to use this function,otherwise it may cause risks.
 * 				Attention description:Flash cannot be powered off while using this function.
 */
_attribute_text_sec_ unsigned char flash_trim_check(void);
#endif /* DRIVERS_B91_FLASH_TRIM_H_ */
