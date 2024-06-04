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

volatile unsigned int cur_32k_tick[200];
volatile unsigned int cur_32k_tick_step[200];
volatile unsigned int cur_32k_track[200];
volatile unsigned int cur_32k_tick_step_err;
volatile unsigned int cur_32k_tick_track_err;
volatile unsigned int stimer_track_32k_value;
volatile unsigned int timer_up_32k_tick;
volatile unsigned int stimer_tick_low3bit;

void user_init(void)
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);

#if (STIMER_MODE == STIMER_IRQ)
	stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S); //set capture tick
	stimer_set_irq_mask(FLD_SYSTEM_IRQ);	//irq enable
	plic_interrupt_enable(IRQ_SYSTIMER);
	core_interrupt_enable();

#elif(MCU_CORE_B93 && (STIMER_MODE == STIMER_IRQ_D25))

	stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1S); //set capture tick
	stimer_set_irq_mask(FLD_SYSTEM_IRQ_MASK_D25F);	//irq enable
	plic_interrupt_enable(IRQ_SYSTIMER);
	core_interrupt_enable();

	sys_n22_set_startup_addr(0x20080000);
    sys_n22_init();

    //enable dsp
	sys_dsp_init();

#elif(MCU_CORE_B93 && (STIMER_MODE == STIMER_IRQ_D25_N22_DSP))

	stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1S); //set capture tick
	stimer_set_irq_capture_n22(stimer_get_tick() + (500*SYSTEM_TIMER_TICK_1MS));
	stimer_set_irq_capture_dsp(stimer_get_tick() + (200*SYSTEM_TIMER_TICK_1MS));
	stimer_set_irq_mask(FLD_SYSTEM_IRQ_MASK_D25F | FLD_SYSTEM_IRQ_MASK_N22 | FLD_SYSTEM_IRQ_MASK_DSP);	//irq enable
	plic_interrupt_enable(IRQ_SYSTIMER);
	core_interrupt_enable();

#elif (STIMER_MODE == STIMER_GET_32K_TICK)
	clock_32k_init(CLK_32K_RC);
	clock_cal_32k_rc();

#elif (STIMER_MODE == STIMER_SET_32K_TICK)
	clock_set_32k_tick(clock_get_32k_tick() + 32000);	//1s
	analog_write_reg8(0x64, 0xff);
#if (MCU_CORE_B91)
	plic_interrupt_enable(IRQ_PM_TM);
#elif (MCU_CORE_B92)
	plic_interrupt_enable(IRQ_PM_IRQ);
#elif (MCU_CORE_B93)
	plic_interrupt_enable(IRQ_PM_WKUP);
#endif
	core_interrupt_enable();

#elif (STIMER_MODE == STIMER_TRACK_32K_TICK)
	clock_32k_init(CLK_32K_RC);
	clock_cal_32k_rc();
	reg_system_ctrl = (reg_system_ctrl & 0x0f) | 0xc0;//2^(16-cal_32k_mode)
	stimer_track_32k_value = 750 * ( 1 << (16 - (reg_system_ctrl>>4)));
	delay_ms(1000);

#endif
}

void main_loop(void)
{
#if (STIMER_MODE == STIMER_DELAY)
	delay_ms(500);
	gpio_toggle(LED1);

#elif(MCU_CORE_B93 && (STIMER_MODE == STIMER_IRQ_D25_N22_DSP))
	delay_ms(20);
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ_N22))
	{
		gpio_toggle(LED3);
		stimer_clr_irq_status(FLD_SYSTEM_IRQ_N22);
		stimer_set_irq_capture_n22(stimer_get_tick() + (500*SYSTEM_TIMER_TICK_1MS));
	}
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ_DSP))
	{
		gpio_toggle(LED4);
		stimer_clr_irq_status(FLD_SYSTEM_IRQ_DSP);
		stimer_set_irq_capture_dsp(stimer_get_tick() + (200*SYSTEM_TIMER_TICK_1MS));
	}

#elif (STIMER_MODE == STIMER_GET_32K_TICK)
	cur_32k_tick[0] = clock_get_32k_tick();
	for(int i=1; i<200; i++)
	{
		delay_ms(100);
		cur_32k_tick[i] = clock_get_32k_tick();
		cur_32k_tick_step[i] = cur_32k_tick[i] - cur_32k_tick[i-1];
		if((cur_32k_tick_step[i] < 0xc7f) || (cur_32k_tick_step[i] > 0xc83))	//0xc81,500ppm
		{
			cur_32k_tick_step_err = i;
			gpio_set_high_level(LED1);
			while(1);
		}
	}

#elif (STIMER_MODE == STIMER_SET_32K_TICK)
	if(timer_up_32k_tick == 1)
	{
		timer_up_32k_tick = 0;
		clock_set_32k_tick(clock_get_32k_tick() + 32000);	//1s
	}

#elif (STIMER_MODE == STIMER_TRACK_32K_TICK)
	for(int i=0; i<200; i++)
	{
		delay_ms(100);
		cur_32k_track[i] = read_reg32(0x140214);
		if((cur_32k_track[i] < (stimer_track_32k_value-0x06)) || (cur_32k_track[i] > (stimer_track_32k_value+0x06)))	//500ppm
		{
			cur_32k_tick_track_err = i;
			gpio_set_high_level(LED1);
			while(1);
		}
	}

#elif (STIMER_MODE == STIMER_GET_TICK)
	if((stimer_get_tick()&(0x07))!= 0x00)
	{
		stimer_tick_low3bit = 1;
		gpio_set_high_level(LED1);
		while(1);
	}

#endif
}

