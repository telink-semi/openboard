/********************************************************************************************************
 * @file     tl_common.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
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
 *******************************************************************************************************/

#ifndef TL_COMMON_H_
#define TL_COMMON_H_


#include "common/types.h"
#include "common/bit.h"
#include "common/utility.h"
#include "common/static_assert.h"
#include "common/assert.h"

#include "application/print/printf.h"
#include "vendor/common/user_config.h"
#include "config.h"

#include <string.h>




#include "common/usb_dbg/myudb.h"
#include "common/usb_dbg/log_def_stack.h"



#include "vendor/common/blt_common.h"
#include "vendor/common/blt_fw_sign.h"
#include "vendor/common/blt_led.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/common/custom_pair.h"
#include "vendor/common/flash_fw_check.h"

#include "gpio_default.h"

#endif /* TL_COMMON_H_ */
