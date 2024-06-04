/********************************************************************************************************
 * @file    app_ir_fifo.c
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
#if(SET_PWM_MODE==PWM_IR_FIFO)

#if(MCU_CORE_B91)
#define PWM_PIN		(PWM_PWM0_PB4)
#define PWM_ID		(get_pwmid(PWM_PIN))
#elif(MCU_CORE_B92)
#define PWM_ID		PWM0_ID
#define PWM_PIN		GPIO_FC_PB4
#define PWM_FUNC     PWM0
#elif(MCU_CORE_B93)
#define PWM_ID		PWM0_ID
#define PWM_PIN		PWM_PWM0_PA0
#define PWM_FUNC    FC_PWM0
#endif


#define PWM_PULSE_NUM1			5
#define PWM_PULSE_NUM2			6


_attribute_ram_code_sec_ void pwm_irq_handler(void)
{

	if(pwm_get_irq_status(FLD_PWM0_IR_FIFO_IRQ ))
	{
		pwm_set_pwm0_ir_fifo_cfg_data(PWM_PULSE_NUM1,1,1);

		pwm_set_pwm0_ir_fifo_cfg_data(PWM_PULSE_NUM2,0,1);

		pwm_clr_irq_status(FLD_PWM0_IR_FIFO_IRQ );

		gpio_toggle(LED4);
	}
	else
	{
		gpio_set_high_level(LED3);
	}

}
PLIC_ISR_REGISTER(pwm_irq_handler, IRQ_PWM)


void user_init(void)
{
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_input_dis(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);

	delay_ms(2000);

	pwm_set_clk((unsigned char) (sys_clk.pclk*1000*1000/PWM_PCLK_SPEED-1));

    pwm_set_tcmp(PWM_ID,50 * CLOCK_PWM_CLOCK_1US);

    pwm_set_tmax(PWM_ID,100 * CLOCK_PWM_CLOCK_1US);

    pwm_set_pwm0_tcmp_and_tmax_shadow(200 * CLOCK_PWM_CLOCK_1US,100 * CLOCK_PWM_CLOCK_1US);

#if(MCU_CORE_B91)
        pwm_set_pin(PWM_PIN);
#elif(MCU_CORE_B92||MCU_CORE_B93)
        pwm_set_pin(PWM_PIN,PWM_FUNC);
#endif

    pwm_set_pwm0_mode(PWM_IR_FIFO_MODE);

    pwm_set_pwm0_ir_fifo_cfg_data(PWM_PULSE_NUM1,1,1);

    pwm_set_pwm0_ir_fifo_cfg_data(PWM_PULSE_NUM2,0,1);

    pwm_clr_irq_status(FLD_PWM0_IR_FIFO_IRQ );

	pwm_set_pwm0_ir_fifo_irq_trig_level(1);

	pwm_set_irq_mask(FLD_PWM0_IR_FIFO_IRQ );

	core_interrupt_enable();

	plic_interrupt_enable(IRQ_PWM);

	delay_us(400);

}


void main_loop(void)
{
    delay_ms(500);

    gpio_toggle(LED2);
}

#endif
