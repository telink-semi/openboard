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

extern void user_init(void);
extern void main_loop (void);
extern volatile unsigned int timer_up_32k_tick;

#if (STIMER_MODE == STIMER_IRQ)
_attribute_ram_code_sec_ void stimer_irq_handler()
{
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ))
	{
		gpio_toggle(LED2);
		stimer_clr_irq_status(FLD_SYSTEM_IRQ);            //clr irq
		stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
	}
}


#elif (STIMER_MODE == STIMER_SET_32K_TICK)
_attribute_ram_code_sec_ void pm_irq_handler()
{
	if(analog_read_reg8(0x64)&(BIT(2)))
	{
		gpio_toggle(LED2);
		timer_up_32k_tick = 1;
	}
	analog_write_reg8(0x64, 0xff);
}

#endif

/**
 * @brief		This is main function
 * @return      none
 */
int main(void)
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
#endif

	user_init();

    while(1)
    {
    	main_loop();
    }

    return 0;
}
