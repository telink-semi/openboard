/********************************************************************************************************
 * @file    app.c
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

unsigned char dat[5]={0};
unsigned char  mdec_test_cnt = 0;

/**
 * @brief		This function serves to handle the interrupt of MCU.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ void pm_irq_handler(void)
{
	if(mdec_get_irq_status(FLD_WKUP_MDEC))
	{
		mdec_test_cnt = 1;
		mdec_clr_irq_status(FLD_WKUP_MDEC);
	}
}
#if (MCU_CORE_B91)
PLIC_ISR_REGISTER(pm_irq_handler, IRQ_PM_TM)
#elif (MCU_CORE_B92)
PLIC_ISR_REGISTER(pm_irq_handler, IRQ_PM_IRQ)
#endif

void user_init(void)
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	clock_cal_32k_rc();
	mdec_init(FLD_SELE_PE0);
	//need to clear interrupt state:FLD_WKUP_TIMER,FLD_WKUP_DIG,avoid MDEC interrupt misjudgment,
	//due to FLD_WKUP_TIMER,FLD_WKUP_DIG is 1,plic receives the result,once enable plic, will request an interrupt to the CPU.
	//Therefore, notice the processing of the interrupt sequence here.changed by shuaixing,confirmed by jianzhi.20201104.
	mdec_clr_irq_status(FLD_WKUP_MDEC|FLD_WKUP_TIMER|FLD_WKUP_DIG);
	plic_interrupt_enable(IRQ_PM_TM);
	plic_interrupt_claim();
	plic_interrupt_complete(IRQ_PM_TM);
	core_interrupt_enable();
	mdec_reset();
}

void main_loop (void)
{
	if(mdec_test_cnt)
	{
		mdec_test_cnt = 0;
		if(mdec_read_dat(dat))
		{
			gpio_toggle(LED2);
			delay_ms(2000);
		}
	}
}

