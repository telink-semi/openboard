/********************************************************************************************************
 * @file    puya_flash_scratch.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef DRIVERS_B91_FLASH_SCRATCH_H_
#define DRIVERS_B91_FLASH_SCRATCH_H_
#include "driver.h"
#include "compiler.h"
#include "puya_common.h"


/**
 * @brief 		This function serves to flash scratch test.
 * @return 		ret - return 1: The chip mid is not 0x156085.
 *                    return 0: test mode finish.
 */
_attribute_text_sec_ unsigned char flash_scratch(void);
/**
 * @brief 		This function serves to flash scratch check.
 * @return 		ret - return 3: The chip mid is not 0x156085.
 *                    return 2: write err in testmode.
 *                    return 1: erase err in common.
 *                    return 0: ok.
 */
_attribute_text_sec_ unsigned char flash_check_scratch(void);
/**
 * @brief 		This function serves to flash erase chip in common mode.
 * @return 		none.
 */
_attribute_text_sec_ void flash_erase_chip(void);

#endif /* DRIVERS_B91_FLASH_TRIM_H_ */
