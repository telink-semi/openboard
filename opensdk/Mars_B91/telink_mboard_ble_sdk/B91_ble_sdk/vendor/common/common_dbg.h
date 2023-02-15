/********************************************************************************************************
 * @file     common_dbg.h
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

#ifndef COMMON_DBG_H_
#define COMMON_DBG_H_



#include "common/utility.h"
#include "vendor/common/user_config.h"


#ifndef APP_DUMP_EN
#define APP_DUMP_EN         						0
#endif



#ifndef EXCEPT_HANDLE_DEBUG_ENABLE
#define EXCEPT_HANDLE_DEBUG_ENABLE         			0
#endif








#if (APP_DUMP_EN)

extern my_fifo_t print_fifo;
extern	unsigned char 	print_fifo_b[];



void app_dmup_debug_init(void);

#endif








#endif /* COMMON_DBG_H_ */
