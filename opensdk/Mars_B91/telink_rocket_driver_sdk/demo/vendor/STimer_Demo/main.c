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

extern void user_init(void);
extern void main_loop (void);
extern volatile unsigned int timer_up_32k_tick;

#if (STIMER_MODE == STIMER_IRQ)
_attribute_ram_code_sec_ void stimer_irq_handler(void)
{
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ))
	{
		gpio_toggle(LED2);
		stimer_clr_irq_status(FLD_SYSTEM_IRQ);            //clr irq
		stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
	}
}
PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)
#elif ( MCU_CORE_B93 && ((STIMER_MODE == STIMER_IRQ_D25) || (STIMER_MODE == STIMER_IRQ_D25_N22_DSP)))
_attribute_ram_code_sec_ void stimer_irq_handler(void)
{
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ_D25F))
	{
		gpio_toggle(LED2);
		stimer_clr_irq_status(FLD_SYSTEM_IRQ_D25F);
		stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
	}
}
PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)
#elif (STIMER_MODE == STIMER_SET_32K_TICK)
#if (MCU_CORE_B91)||(MCU_CORE_B92)
_attribute_ram_code_sec_ void pm_irq_handler(void)
{
	if(analog_read_reg8(0x64)&(BIT(2)))
	{
		gpio_toggle(LED2);
		timer_up_32k_tick = 1;
	}
	analog_write_reg8(0x64, 0xff);
}
#if (MCU_CORE_B91)
PLIC_ISR_REGISTER(pm_irq_handler, IRQ_PM_TM)
#elif (MCU_CORE_B92)
PLIC_ISR_REGISTER(pm_irq_handler, IRQ_PM_IRQ)
#endif
#elif (MCU_CORE_B93)
PLIC_ISR_REGISTER(pm_wkup_irq_handler, IRQ_PM_WKUP)
#endif

#endif

/**
 * @brief		This is main function
 * @return      none
 */
int main(void)
{
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    while(1)
    {
    	main_loop();
    }

    return 0;
}
