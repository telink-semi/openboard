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
int timer0_irq_cnt = 0;
int timer1_irq_cnt = 0;
volatile unsigned int timer0_gpio_width =0;
volatile unsigned int timer1_gpio_width =0;

_attribute_ram_code_sec_ void timer0_irq_handler(void)
{
#if( TIMER_MODE == TIMER_SYS_CLOCK_MODE )

	if(timer_get_irq_status(TMR_STA_TMR0))
	{
		timer_clr_irq_status(TMR_STA_TMR0);
        gpio_toggle(LED2);
		timer0_irq_cnt ++;
	}

#elif(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	if(timer_get_irq_status(TMR_STA_TMR0))
	{
		timer_clr_irq_status(TMR_STA_TMR0); //clear irq status

		gpio_toggle(LED2);

		timer0_irq_cnt ++;
	}

#elif(TIMER_MODE == TIMER_GPIO_WIDTH_MODE)

	if(timer_get_irq_status(TMR_STA_TMR0))
	{
		timer_clr_irq_status(TMR_STA_TMR0); //clear irq status
        timer0_gpio_width = timer0_get_gpio_width();
        timer0_set_tick(0);
		gpio_toggle(LED2);
	}

#endif
}

_attribute_ram_code_sec_ void timer1_irq_handler(void)
{
#if( TIMER_MODE == TIMER_SYS_CLOCK_MODE )

	if(timer_get_irq_status(TMR_STA_TMR1))
	{
		timer_clr_irq_status(TMR_STA_TMR1);
		gpio_toggle(LED3);
		timer1_irq_cnt ++;
	}

#elif(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	if(timer_get_irq_status(TMR_STA_TMR1))
	{
		timer_clr_irq_status(TMR_STA_TMR1);//clear irq status
		gpio_toggle(LED3);
		timer1_irq_cnt ++;
	}

#elif(TIMER_MODE == TIMER_GPIO_WIDTH_MODE)

	if(timer_get_irq_status(TMR_STA_TMR1))
	{
		timer_clr_irq_status(TMR_STA_TMR1);//clear irq status
		timer1_gpio_width = timer1_get_gpio_width();
		timer1_set_tick(0);
		gpio_toggle(LED3);
	}

#endif
}

/**
 * @brief		This is main function
 * @param[in]	none
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
