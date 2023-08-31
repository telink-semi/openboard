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
#include "printf.h"

void user_init(void)
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);

	lpc_set_input_chn(LPC_INPUT_PB1);
	//When the chip works in low power mode, the reference voltage can only be provided by UVLO or from PB0 and PB3.
	lpc_set_input_ref(LPC_NORMAL,LPC_REF_872MV);
	lpc_set_scaling_coeff(LPC_SCALING_PER50);
	//LPC POWER must be turned on last.
	lpc_power_on();
	//The LPC sampling clock source is RC 32K. After turning on the LPC POWER, you must wait for two sampling periods to obtain the sampling value.
	delay_us(64);
}


void main_loop(void)
{
	printf("result = %d\r\n",lpc_get_result());

	gpio_toggle(LED1);
	delay_ms(200);
}


