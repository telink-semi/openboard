/********************************************************************************************************
 * @file    main.c
 *
 * @brief   This is the source file for B91m
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
#include "app_config.h"
#include "calibration.h"
#include "gpio_default.h"
extern void user_init();
extern void main_loop (void);

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)   //must on ramcode
{
#if(MCU_CORE_B91)
	sys_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	//Note: This function can improve the performance of some modules, which is described in the function comments.
	//Called immediately after sys_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
	CCLK_24M_HCLK_24M_PCLK_24M;
#elif(MCU_CORE_B92)
	sys_init(LDO_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3);
	wd_32k_stop();
	//Note: This function can improve the performance of some modules, which is described in the function comments.
	//Called immediately after sys_init, set in other positions, some calibration values may not take effect.
	calibration_func(GPIO_VOLTAGE_3V3);
	CCLK_24M_HCLK_24M_PCLK_24M;
#elif(MCU_CORE_B93)
	sys_init();
	wd_32k_stop();
#endif
	//gpio_init(1);
	user_init();
	while (1) {
		main_loop ();
	}
	return 0;
}
