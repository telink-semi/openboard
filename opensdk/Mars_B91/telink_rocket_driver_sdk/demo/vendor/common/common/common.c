/********************************************************************************************************
 * @file    common.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "common.h"

#if(MCU_CORE_B91)
void platform_init(power_mode_e power_mode, vbat_type_e vbat_v)
#elif(MCU_CORE_B92)
void platform_init(power_mode_e power_mode, vbat_type_e vbat_v, gpio_voltage_e gpio_v)
#else
void platform_init(void)
#endif
{
/**
	===============================================================================
						 ##### sys_init #####
	===============================================================================
*/
#if (MCU_CORE_B91)
    sys_init(power_mode, vbat_v);
#elif (MCU_CORE_B92)
    sys_init(power_mode, vbat_v, gpio_v);
#else
    sys_init();
#endif
/**
    ===============================================================================
                         ##### set SWS pull #####
    ===============================================================================
    If SWS is not set up,SWS will be floating,
    causing abnormal sleep currents of suspend,
    there may be the risk of SWS miswriting the chip registers or sram causing a crash.
    ===============================================================================
*/
    gpio_set_up_down_res(GPIO_SWS, GPIO_PIN_PULLUP_1M);

/**
	===============================================================================
						 ##### 32k watchdog stop #####
	===============================================================================
	For chips with 32K watchdog function, the 32K watchdog function is enabled by default.
	If the program does not handle the watchdog, the chip will reset in 5s,
	and the driver will turn it off by default. If you want to use this function,
	you can replace this place with the dog feeder interface.
	===============================================================================
*/
#if (!MCU_CORE_B91 )
    wd_32k_stop();
#endif
/**
	===============================================================================
						 ##### calibration #####
	===============================================================================
	This function can improve the performance of some modules,
	which is described in the function comments.
	Called immediately after sys_init,set in other positions,
	some calibration values may not take effect.
	===============================================================================
*/
#if (MCU_CORE_B91)
    user_read_flash_value_calib();
#elif(MCU_CORE_B92)
    calibration_func(gpio_v);
#endif

}

