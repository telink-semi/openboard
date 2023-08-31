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
#if TEST_DEMO == BQB_DEMO
#include "BQB/bqb.h"
#endif


#if TEST_DEMO==EMI_DEMO && EMI_SUPPORT_SETTING
extern void emi_setting_init(void);
extern emi_setting_t g_emi_setting;
#endif

extern void user_init(void);
extern void main_loop (void);
#if TEST_DEMO==BQB_DEMO && SUPPORT_CONFIGURATION
extern void rd_usr_definition(unsigned char _s);
#endif

#if(MCU_CORE_B91)
#define POWER_MODE_LDO1V4_LDO1V8		0
#define POWER_MODE_DCDC1V4_DCDC1V8		1
#define POWER_MODE_DCDC1V4_LDO1V8		2

/**
 * @brief		This is macro used to set the initialize power mode.
 */
#define POWER_MODE_SELECT		POWER_MODE_LDO1V4_LDO1V8
#elif(MCU_CORE_B92)
#define POWER_MODE_LDO1V2_LDO2V0		0
#define POWER_MODE_DCDC1V2_LDO2V0		1
#define POWER_MODE_DCDC1V2_DCDC2V0	2

/**
 * @brief		This is macro used to set the initialize power mode.
 */
#define POWER_MODE_SELECT		POWER_MODE_LDO1V2_LDO2V0
#endif

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main(void)
{
#if TEST_DEMO==BQB_DEMO
	#if SUPPORT_CONFIGURATION
		rd_usr_definition(1);
		if(usr_config.power_mode == 0)
		{
#if(MCU_CORE_B91)
			sys_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif(MCU_CORE_B92)
			sys_init(LDO_1P4_LDO_2P0,VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);
#endif
		}
		else if(usr_config.power_mode == 1)
		{
#if(MCU_CORE_B91)
			sys_init(DCDC_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif(MCU_CORE_B92)

			sys_init(DCDC_1P4_LDO_2P0,VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);

#endif
		}
		else
		{
#if(MCU_CORE_B91)
			sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif(MCU_CORE_B92)

			sys_init(DCDC_1P4_DCDC_2P0,VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);

#endif
		}
	#else
		#if SWITCH_POWER_MODE
#if(MCU_CORE_B91)
			sys_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif(MCU_CORE_B92)
			sys_init(LDO_1P4_LDO_2P0,VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);
#endif
		#else
#if(MCU_CORE_B91)
			sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif(MCU_CORE_B92)

			sys_init(DCDC_1P4_DCDC_2P0,VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);

#endif
		#endif
	#endif
#if(MCU_CORE_B92)
			wd_32k_stop();
#endif	
#else
#if(MCU_CORE_B91)

#if TEST_DEMO==EMI_DEMO && EMI_SUPPORT_SETTING
	emi_setting_init();
	if(g_emi_setting.general_setting.power_mode == 1)
	{
		sys_init(DCDC_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	}
	else if(g_emi_setting.general_setting.power_mode == 2)
	{
		sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	}
	else
	{
		sys_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	}
#elif TEST_DEMO==EMI_DEMO


#if POWER_MODE_SELECT == POWER_MODE_LDO1V4_LDO1V8
	sys_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif POWER_MODE_SELECT == POWER_MODE_DCDC1V4_DCDC1V8
	sys_init(DCDC_1P4_DCDC_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#elif POWER_MODE_SELECT == POWER_MODE_DCDC1V4_LDO1V8
	sys_init(DCDC_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
#endif

#endif


	//Note: This function can improve the performance of some modules, which is described in the function comments.
	//Called immediately after sys_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
#elif(MCU_CORE_B92)

#if TEST_DEMO==EMI_DEMO && EMI_SUPPORT_SETTING
	emi_setting_init();

	if(g_emi_setting.general_setting.power_mode == 1)
	{
		sys_init(DCDC_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);
	}
	else if(g_emi_setting.general_setting.power_mode == 2)
	{
		sys_init(DCDC_1P4_DCDC_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);
	}
	else
	{
		sys_init(LDO_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);
	}

#elif TEST_DEMO==EMI_DEMO

#if POWER_MODE_SELECT == POWER_MODE_LDO1V2_LDO2V0
	sys_init(LDO_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);
#elif POWER_MODE_SELECT == POWER_MODE_DCDC1V2_LDO2V0
	sys_init(DCDC_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);
#elif POWER_MODE_SELECT == POWER_MODE_DCDC1V2_DCDC2V0
	sys_init(DCDC_1P4_DCDC_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6,GPIO_VOLTAGE_3V3);

#endif

#endif

#if(MCU_CORE_B92)
	wd_32k_stop();
	//Note: This function can improve the performance of some modules, which is described in the function comments.
	//Called immediately after sys_init, set in other positions, some calibration values may not take effect.
	calibration_func(GPIO_VOLTAGE_3V3);
#endif
#endif
#endif

	CCLK_24M_HCLK_24M_PCLK_24M;

    user_init();

    while(1)
    {
    	main_loop();
    }
    return 0;
}


