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
#include "compiler.h"

unsigned char dat[5] = {0};
unsigned char result = 0;

_attribute_data_retention_sec_ unsigned int retention_data_test = 0;

void user_init(void)
{
	delay_ms(2000);

#if CURRENT_TEST
	gpio_shutdown(GPIO_ALL);
	//Note:
	//During current test, sws needs to be given a fixed level.
	//If sws is not connected to the analog pull up and down,it is in a suspended state and may be in a level indeterminate state,
	//which will affect the electric leakage of digital and lead to higher current than normal during suspend sleep.
	gpio_set_up_down_res(GPIO_PA7, GPIO_PIN_PULLUP_1M);


#else
	// init the LED pin, for indication
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);
	gpio_set_high_level(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_set_low_level(LED2);
	delay_ms(1000);
#if(MCU_CORE_B91)
#if ((PM_MODE == SUSPEND_MDEC_WAKEUP)||(PM_MODE == DEEP_MDEC_WAKEUP)||(PM_MODE == DEEP_RET32K_MDEC_WAKEUP)||(PM_MODE == DEEP_RET64K_MDEC_WAKEUP))
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_input_dis(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	if(((pm_get_wakeup_src()) & PM_WAKEUP_MDEC) && (CRC_OK == mdec_read_dat(dat)))
	{
		gpio_set_high_level(LED3);
		delay_ms(2000);
	}
#endif
#endif
#if ((PM_MODE == SUSPEND_COMPARATOR_WAKEUP)||(PM_MODE == DEEP_COMPARATOR_WAKEUP)||(PM_MODE == DEEP_RET32K_COMPARATOR_WAKEUP)||(PM_MODE == DEEP_RET64K_COMPARATOR_WAKEUP)||(PM_MODE == DEEP_RET96K_COMPARATOR_WAKEUP))
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_input_dis(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	result = lpc_get_result();
	if(result){
		gpio_set_high_level(LED3);
	}else{
		gpio_set_low_level(LED3);
	}
	delay_ms(2000);
#endif

#endif

	//24M RC is inaccurate, and it is greatly affected by temperature, so real-time calibration is required
	//The 24M RC needs to be calibrated before the pm_sleep_wakeup function,
	//because this clock will be used to kick 24m xtal start after wake up,
	//The more accurate this time, the faster the crystal will start.Calibration cycle depends on usage
	clock_cal_24m_rc();

	//If it is timer or mdec wake up, you need to initialize 32K
#if(MCU_CORE_B91)
	if(PM_MODE == SUSPEND_32K_RC_WAKEUP || PM_MODE == DEEP_32K_RC_WAKEUP || PM_MODE == DEEP_RET32K_32K_RC_WAKEUP || PM_MODE == DEEP_RET64K_32K_RC_WAKEUP \
	|| PM_MODE == SUSPEND_MDEC_WAKEUP || PM_MODE == DEEP_MDEC_WAKEUP || PM_MODE == DEEP_RET32K_MDEC_WAKEUP || PM_MODE == DEEP_RET64K_MDEC_WAKEUP \
	|| PM_MODE == SUSPEND_COMPARATOR_WAKEUP || PM_MODE == DEEP_COMPARATOR_WAKEUP || PM_MODE == DEEP_RET32K_COMPARATOR_WAKEUP || PM_MODE == DEEP_RET64K_COMPARATOR_WAKEUP )
	{
		clock_32k_init(CLK_32K_RC);
		clock_cal_32k_rc();	//6.68ms
	}
	else if(PM_MODE == SUSPEND_32K_XTAL_WAKEUP || PM_MODE == DEEP_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP)
	{
		clock_32k_init(CLK_32K_XTAL);
		clock_kick_32k_xtal(10);
	}
#elif(MCU_CORE_B92)
	if(PM_MODE == SUSPEND_32K_RC_WAKEUP || PM_MODE == DEEP_32K_RC_WAKEUP || PM_MODE == DEEP_RET32K_32K_RC_WAKEUP || PM_MODE == DEEP_RET64K_32K_RC_WAKEUP || PM_MODE == DEEP_RET96K_32K_RC_WAKEUP \
	|| PM_MODE == SUSPEND_COMPARATOR_WAKEUP || PM_MODE == DEEP_COMPARATOR_WAKEUP || PM_MODE == DEEP_RET32K_COMPARATOR_WAKEUP || PM_MODE == DEEP_RET64K_COMPARATOR_WAKEUP || PM_MODE == DEEP_RET96K_COMPARATOR_WAKEUP)
	{
		clock_32k_init(CLK_32K_RC);
		clock_cal_32k_rc();	//6.68ms
	}
	else if(PM_MODE == SUSPEND_32K_XTAL_WAKEUP || PM_MODE == DEEP_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP || PM_MODE == DEEP_RET96K_32K_XTAL_WAKEUP)
	{
		clock_32k_init(CLK_32K_XTAL);
		clock_kick_32k_xtal(10);
	}
#endif

#if(PM_MODE == IDLE_TIMER_WAKEUP)



#elif(PM_MODE == SUSPEND_PAD_WAKEUP)	//Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC.
	pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_HIGH, 1);
	gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLDOWN_100K);

#elif(PM_MODE == DEEP_PAD_WAKEUP)	//Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC.
	pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_HIGH, 1);
	gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLDOWN_100K);
	pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, PM_TICK_STIMER, 0);

#elif(PM_MODE == DEEP_RET32K_PAD_WAKEUP)
	retention_data_test++;
	pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_HIGH, 1);
	gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLDOWN_100K);
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K, PM_WAKEUP_PAD, PM_TICK_STIMER, 0);

#elif(PM_MODE == DEEP_RET64K_PAD_WAKEUP)
	retention_data_test++;
	pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_HIGH, 1);
	gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLDOWN_100K);
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW64K, PM_WAKEUP_PAD, PM_TICK_STIMER, 0);

#elif(PM_MODE == DEEP_RET96K_PAD_WAKEUP)
	retention_data_test++;
	pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_HIGH, 1);
	gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLDOWN_100K);
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW96K, PM_WAKEUP_PAD, PM_TICK_STIMER, 0);

#elif(PM_MODE==DEEP_32K_RC_WAKEUP||PM_MODE==DEEP_32K_XTAL_WAKEUP)
	pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_TIMER, PM_TICK_STIMER, (stimer_get_tick() + 4000*SYSTEM_TIMER_TICK_1MS));

#elif(PM_MODE == DEEP_RET32K_32K_RC_WAKEUP || PM_MODE == DEEP_RET32K_32K_XTAL_WAKEUP)
	retention_data_test++;
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K, PM_WAKEUP_TIMER, PM_TICK_STIMER, (stimer_get_tick() + 4000*SYSTEM_TIMER_TICK_1MS));

#elif(PM_MODE == DEEP_RET64K_32K_RC_WAKEUP || PM_MODE == DEEP_RET64K_32K_XTAL_WAKEUP)
	retention_data_test++;
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW64K, PM_WAKEUP_TIMER, PM_TICK_STIMER, (stimer_get_tick() + 4000*SYSTEM_TIMER_TICK_1MS));

#elif(PM_MODE == DEEP_RET96K_32K_RC_WAKEUP || PM_MODE == DEEP_RET96K_32K_XTAL_WAKEUP)
	retention_data_test++;
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW96K, PM_WAKEUP_TIMER, PM_TICK_STIMER, (stimer_get_tick() + 4000*SYSTEM_TIMER_TICK_1MS));

#elif((MCU_CORE_B91)&&(PM_MODE == SUSPEND_MDEC_WAKEUP))
	mdec_init(FLD_SELE_PE0);
	mdec_reset();
	pm_set_mdec_value_wakeup(MDEC_MATCH_VALUE);

#elif((MCU_CORE_B91)&&(PM_MODE == DEEP_MDEC_WAKEUP))
	mdec_init(FLD_SELE_PE0);
	mdec_reset();
	pm_set_mdec_value_wakeup(MDEC_MATCH_VALUE);
	pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_MDEC, PM_TICK_STIMER, 0);

#elif((MCU_CORE_B91)&&(PM_MODE == DEEP_RET32K_MDEC_WAKEUP))
	retention_data_test++;
	mdec_init(FLD_SELE_PE0);
	mdec_reset();
	pm_set_mdec_value_wakeup(MDEC_MATCH_VALUE);
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K, PM_WAKEUP_MDEC, PM_TICK_STIMER, 0);

#elif((MCU_CORE_B91)&&(PM_MODE == DEEP_RET64K_MDEC_WAKEUP))
	retention_data_test++;
	mdec_init(FLD_SELE_PE0);
	mdec_reset();
	pm_set_mdec_value_wakeup(MDEC_MATCH_VALUE);
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW64K, PM_WAKEUP_MDEC, PM_TICK_STIMER, 0);

#elif(PM_MODE == SUSPEND_COMPARATOR_WAKEUP)
	lpc_set_input_chn(LPC_INPUT_PB1);
	lpc_set_input_ref(LPC_LOWPOWER,LPC_REF_820MV);
	lpc_set_scaling_coeff(LPC_SCALING_PER75);
	lpc_power_on();
	delay_us(64);

#elif(PM_MODE == DEEP_COMPARATOR_WAKEUP)
	lpc_set_input_chn(LPC_INPUT_PB1);
	lpc_set_input_ref(LPC_LOWPOWER,LPC_REF_820MV);
	lpc_set_scaling_coeff(LPC_SCALING_PER75);
	lpc_power_on();
	delay_us(64);
	//When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV,
	//otherwise can not enter sleep normally, crash occurs.
	pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_COMPARATOR, PM_TICK_STIMER, 0);

#elif(PM_MODE == DEEP_RET32K_COMPARATOR_WAKEUP)
	retention_data_test++;
	lpc_set_input_chn(LPC_INPUT_PB1);
	lpc_set_input_ref(LPC_LOWPOWER,LPC_REF_820MV);
	lpc_set_scaling_coeff(LPC_SCALING_PER75);
	lpc_power_on();
	delay_us(64);
	//When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV,
	//otherwise can not enter sleep normally, crash occurs.
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K, PM_WAKEUP_COMPARATOR, PM_TICK_STIMER, 0);

#elif(PM_MODE == DEEP_RET64K_COMPARATOR_WAKEUP)
	retention_data_test++;
	lpc_set_input_chn(LPC_INPUT_PB1);
	lpc_set_input_ref(LPC_LOWPOWER,LPC_REF_820MV);
	lpc_set_scaling_coeff(LPC_SCALING_PER75);
	lpc_power_on();
	delay_us(64);
	//When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV,
	//otherwise can not enter sleep normally, crash occurs.
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW64K, PM_WAKEUP_COMPARATOR, PM_TICK_STIMER, 0);

#elif(PM_MODE == DEEP_RET96K_COMPARATOR_WAKEUP)
	retention_data_test++;
	lpc_set_input_chn(LPC_INPUT_PB1);
	lpc_set_input_ref(LPC_LOWPOWER,LPC_REF_820MV);
	lpc_set_scaling_coeff(LPC_SCALING_PER75);
	lpc_power_on();
	delay_us(64);
	//When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV,
	//otherwise can not enter sleep normally, crash occurs.
	pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW96K, PM_WAKEUP_COMPARATOR, PM_TICK_STIMER, 0);

#elif(PM_MODE == SUSPEND_CORE_USB_WAKEUP)
	usb_set_pin_en();
	reg_wakeup_en |= 0x01;
	gpio_set_up_down_res(GPIO_PA5, GPIO_PIN_PULLDOWN_100K);
	gpio_set_up_down_res(GPIO_PA6, GPIO_PIN_PULLUP_10K);
	pm_set_suspend_power_cfg(PM_POWER_USB, 1);

//#elif(PM_MODE == SUSPEND_CORE_GPIO_WAKEUP)
//	gpio_input_en(WAKEUP_PAD);
//	gpio_irq_en(WAKEUP_PAD);
//	reg_gpio_irq_ctrl |= FLD_GPIO_CORE_WAKEUP_EN;
//	reg_wakeup_en |= 0x02;
//	pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_HIGH, 1);
//	gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLDOWN_100K);

#endif

}


void main_loop(void)
{
	clock_cal_24m_rc();

#if !CURRENT_TEST
	gpio_set_low_level(LED2);
	delay_ms(500);
#endif

#if(PM_MODE == SUSPEND_PAD_WAKEUP)
	pm_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_PAD, PM_TICK_STIMER, 0);

#elif(PM_MODE == SUSPEND_32K_RC_WAKEUP || PM_MODE == SUSPEND_32K_XTAL_WAKEUP)
	pm_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, PM_TICK_STIMER, stimer_get_tick() + 4000*SYSTEM_TIMER_TICK_1MS);

#elif(PM_MODE == SUSPEND_COMPARATOR_WAKEUP)
	//When entering sleep, keep the input voltage and reference voltage difference must be greater than 30mV,
	//otherwise can not enter sleep normally, crash occurs.
	pm_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_COMPARATOR, PM_TICK_STIMER, 0);
	if(1 == lpc_get_result())
	{
		gpio_toggle(LED3);
	}

#elif( (MCU_CORE_B91) && (PM_MODE == SUSPEND_MDEC_WAKEUP))
	pm_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_MDEC, PM_TICK_STIMER, 0);
	if((CRC_OK == mdec_read_dat(dat)))
	{
		gpio_toggle(LED3);
	}
	mdec_reset();
//#elif(PM_MODE == SUSPEND_CORE_USB_WAKEUP || PM_MODE == SUSPEND_CORE_GPIO_WAKEUP)
#elif(PM_MODE == SUSPEND_CORE_USB_WAKEUP )
	pm_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_CORE, PM_TICK_STIMER, 0);
#else
	gpio_set_high_level(LED2);

#endif

#if !CURRENT_TEST
	gpio_set_high_level(LED2);
#endif

	delay_ms(2000);
}

