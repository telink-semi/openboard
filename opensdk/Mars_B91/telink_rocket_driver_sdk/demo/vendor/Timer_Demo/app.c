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
#include <printf.h>
/**************************notices******************************************
 * ********timer clock use APB clock ******************/
#define TIMER_MODE_GPIO_TRIGGER_TICK    0x01
volatile unsigned int t0;
void user_init(void)
{
	gpio_function_en(LED1);
	gpio_function_en(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED1);
	gpio_output_en(LED2);
	gpio_output_en(LED3);

#if (TIMER_MODE == TIMER_SYS_CLOCK_MODE)
	plic_interrupt_enable(IRQ_TIMER0);
	plic_interrupt_enable(IRQ_TIMER1);
	core_interrupt_enable();
	timer_set_init_tick(TIMER0,0);
	timer_set_cap_tick(TIMER0,50*sys_clk.pclk*1000);
	timer_set_init_tick(TIMER1,0);
	timer_set_cap_tick(TIMER1,50*sys_clk.pclk*1000);
	timer_set_mode(TIMER0, TIMER_MODE_SYSCLK);
	timer_set_mode(TIMER1, TIMER_MODE_SYSCLK);
	timer_start(TIMER1);
	timer_start(TIMER0);

#elif(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ_TIMER0);
	plic_interrupt_enable(IRQ_TIMER1);

	gpio_function_en(GPIO_PA2|GPIO_PA3);
	gpio_output_en(GPIO_PA2|GPIO_PA3);
	gpio_set_low_level(GPIO_PA2|GPIO_PA3);

	/****  timer0 POL_RISING  TIMER_GPIO_1 link PA2  **/
	timer_gpio_init(TIMER0, TIMER_GPIO_1,POL_RISING);
	timer_set_init_tick(TIMER0,0);
	timer_set_cap_tick(TIMER0,TIMER_MODE_GPIO_TRIGGER_TICK);
	timer_set_mode(TIMER0, TIMER_MODE_GPIO_TRIGGER);
	timer_start(TIMER0);

	/****  timer1  POL_RISING  TIMER_GPIO_2 link PA3  **/
	timer_gpio_init(TIMER1, TIMER_GPIO_2,POL_RISING);
	timer_set_init_tick(TIMER1,0);
	timer_set_cap_tick(TIMER1,TIMER_MODE_GPIO_TRIGGER_TICK);
	timer_set_mode(TIMER1, TIMER_MODE_GPIO_TRIGGER);
	timer_start(TIMER1);

#elif(TIMER_MODE == TIMER_GPIO_WIDTH_MODE)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ_TIMER0);
	plic_interrupt_enable(IRQ_TIMER1);

	gpio_function_en(GPIO_PA2|GPIO_PA3);
	gpio_output_en(GPIO_PA2|GPIO_PA3);

	/****  timer0 POL_FALLING  TIMER_GPIO_1 link PA2  **/
	gpio_set_high_level(GPIO_PA2);
	delay_ms(50);
	timer_gpio_init(TIMER0, TIMER_GPIO_1, POL_FALLING);
	timer_set_init_tick(TIMER0,0);
	timer_set_cap_tick(TIMER0,0);
	timer_set_mode(TIMER0, TIMER_MODE_GPIO_WIDTH);
	timer_start(TIMER0);
	gpio_set_low_level(GPIO_PA2);
	delay_ms(250);
	gpio_set_high_level(GPIO_PA2);

	/****  timer1  POL_RISING  TIMER_GPIO_2 link PA3  **/
	gpio_set_low_level(GPIO_PA3);
	delay_ms(50);
	timer_gpio_init(TIMER1, TIMER_GPIO_2, POL_RISING);
	timer_set_init_tick(TIMER1,0);
	timer_set_cap_tick(TIMER1,0);
	timer_set_mode(TIMER1, TIMER_MODE_GPIO_WIDTH);
	timer_start(TIMER1);
	gpio_set_high_level(GPIO_PA3);
	delay_ms(250);
	gpio_set_low_level(GPIO_PA3);

#elif(TIMER_MODE == TIMER_TICK_MODE)
	timer_set_init_tick(TIMER0,0);
	timer_set_cap_tick(TIMER0,0);
	timer_set_mode(TIMER0, TIMER_MODE_TICK);//cpu clock tick.
	timer_start(TIMER0);

#elif(TIMER_MODE == TIMER_WATCHDOG_MODE)
	delay_ms(500);
	//eagle will enter deep once after reboot, so the watchdog status cannot be read.
	if(wd_get_status())
	{
		gpio_set_high_level(LED1);
		wd_clear_status();
	}
	wd_set_interval_ms(1000);
	wd_start();

#elif(TIMER_MODE == TIMER_32K_WATCHDOG_MODE)
#if(MCU_CORE_B92||  MCU_CORE_B93)
	delay_ms(500);
	//Remove the stop 32k watchdog operation in main, otherwise this state cannot be read.
	if(wd_32k_get_status())
	{
		gpio_set_high_level(LED1);
		wd_32k_clear_status();
	}
	wd_32k_stop();
#if(WATCHDOG_MODE == WATCHDOG_32K_RC_MODE)
	clock_32k_init(CLK_32K_RC);
	clock_cal_32k_rc();	//6.68ms
#elif(WATCHDOG_MODE == WATCHDOG_32K_XTAL_MODE)
	clock_32k_init(CLK_32K_XTAL);
	clock_kick_32k_xtal(10);
#endif
	wd_32k_set_interval_ms(1000);
	wd_32k_start();

#endif
#endif
}


void main_loop(void)
{
#if(TIMER_MODE == TIMER_GPIO_TRIGGER_MODE)

	gpio_toggle(GPIO_PA2);
	gpio_toggle(GPIO_PA3);

#elif(TIMER_MODE == TIMER_TICK_MODE)

	if(timer0_get_tick() > 500 * sys_clk.pclk*1000)
	{
		timer0_set_tick(0);
		gpio_toggle(LED2);
		gpio_toggle(LED3);
	}

#endif

#if(TIMER_MODE == TIMER_WATCHDOG_MODE)
	//990ms<1000ms, watchdog does not overflow and the program continues to run.
	delay_ms(990);
	wd_clear();
	gpio_set_high_level(LED2);
	//1100ms>1000ms, watchdog overflows, program restarts.
	delay_ms(1100);
	gpio_set_high_level(LED3);

#elif(TIMER_MODE == TIMER_32K_WATCHDOG_MODE)
#if(MCU_CORE_B92||  MCU_CORE_B93)
	//990ms<1000ms, watchdog does not overflow and the program continues to run.
	delay_ms(990);
	wd_32k_stop();
	gpio_set_high_level(LED2);
	wd_32k_set_interval_ms(1000);
	wd_32k_start();
	//1100ms>1000ms, watchdog overflows, program restarts.
	delay_ms(1100);
	gpio_set_high_level(LED3);
#endif
#else

	delay_ms(500);
	gpio_toggle(LED1);

#endif
}

