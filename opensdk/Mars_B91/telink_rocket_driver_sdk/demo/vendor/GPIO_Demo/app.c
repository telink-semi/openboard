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

#if(DEMO_MODE == NORMAL_MODE)

volatile unsigned int gpio_irq_cnt=0,gpio_irq_risc0_cnt=0,gpio_irq_risc1_cnt=0;

#if(MCU_CORE_B92||MCU_CORE_B93||MCU_CORE_B95)
volatile unsigned int gpio_src_irq0_cnt=0;
volatile unsigned int gpio_src_irq1_cnt=0;
volatile unsigned int gpio_src_irq2_cnt=0;
volatile unsigned int gpio_src_irq3_cnt=0;
volatile unsigned int gpio_src_irq4_cnt=0;
volatile unsigned int gpio_src_irq5_cnt=0;
volatile unsigned int gpio_src_irq6_cnt=0;
volatile unsigned int gpio_src_irq7_cnt=0;
#endif

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
#if (GPIO_MODE == GPIO_IRQ )
_attribute_ram_code_sec_noinline_ void gpio_irq_handler(void)
{
	gpio_set_high_level(LED2);
	gpio_irq_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_CLR);
}
PLIC_ISR_REGISTER(gpio_irq_handler, IRQ_GPIO)

#elif(GPIO_MODE == GPIO_IRQ_RISC0)
_attribute_ram_code_sec_noinline_ void gpio_risc0_irq_handler(void)
{
	gpio_set_high_level(LED3);
	gpio_irq_risc0_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR);
}
PLIC_ISR_REGISTER(gpio_risc0_irq_handler, IRQ_GPIO2RISC0)

#elif(GPIO_MODE == GPIO_IRQ_RISC1)
_attribute_ram_code_sec_noinline_ void gpio_risc1_irq_handler(void)
{
	gpio_set_high_level(LED4);
	gpio_irq_risc1_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC1_CLR);
}
PLIC_ISR_REGISTER(gpio_risc1_irq_handler, IRQ_GPIO2RISC1)
#endif

#if(MCU_CORE_B92||MCU_CORE_B93||MCU_CORE_B95)
#if(GPIO_MODE == GPIO_SEL_IRQ_SRC)
_attribute_ram_code_sec_noinline_ void gpio_src0_irq_handler(void)
{
	gpio_src_irq0_cnt++;
	gpio_set_high_level(LED1);
 	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ0);
}
PLIC_ISR_REGISTER(gpio_src0_irq_handler, IRQ_GPIO_SRC0)

_attribute_ram_code_sec_noinline_ void gpio_src1_irq_handler(void)
{
	gpio_src_irq1_cnt++;
	gpio_set_high_level(LED1);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ1);
}
PLIC_ISR_REGISTER(gpio_src1_irq_handler, IRQ_GPIO_SRC1)

_attribute_ram_code_sec_noinline_ void gpio_src2_irq_handler(void)
{
	gpio_src_irq2_cnt++;
	gpio_set_high_level(LED1);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ2);
}
PLIC_ISR_REGISTER(gpio_src2_irq_handler, IRQ_GPIO_SRC2)

_attribute_ram_code_sec_noinline_ void gpio_src3_irq_handler(void)
{
	gpio_src_irq3_cnt++;
	gpio_set_high_level(LED1);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ3);
}
PLIC_ISR_REGISTER(gpio_src3_irq_handler, IRQ_GPIO_SRC3)

_attribute_ram_code_sec_noinline_ void gpio_src4_irq_handler(void)
{
	gpio_src_irq4_cnt++;
	gpio_set_high_level(LED1);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ4);
}
PLIC_ISR_REGISTER(gpio_src4_irq_handler, IRQ_GPIO_SRC4)

_attribute_ram_code_sec_noinline_ void gpio_src5_irq_handler(void)
{
	gpio_src_irq5_cnt++;
	gpio_set_high_level(LED1);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ5);
}
PLIC_ISR_REGISTER(gpio_src5_irq_handler, IRQ_GPIO_SRC5)

_attribute_ram_code_sec_noinline_ void gpio_src6_irq_handler(void)
{
	gpio_src_irq6_cnt++;
	gpio_set_high_level(LED1);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ6);
}
PLIC_ISR_REGISTER(gpio_src6_irq_handler, IRQ_GPIO_SRC6)

_attribute_ram_code_sec_noinline_ void gpio_src7_irq_handler(void)
{
	gpio_src_irq7_cnt++;
	gpio_set_high_level(LED1);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ7);
}
PLIC_ISR_REGISTER(gpio_src7_irq_handler, IRQ_GPIO_SRC7)
#endif
#endif


void user_init(void)
{
#if(MCU_CORE_B91||MCU_CORE_B92)
	delay_ms(1000);

#if(GPIO_MODE == GPIO_HIGH_RESISTOR)
	gpio_shutdown(GPIO_ALL);//set all gpio as high resistor except sws and mspi
#else
	gpio_function_en(LED1);
	gpio_function_en(LED2);
	gpio_function_en(LED3);
	gpio_function_en(LED4);

	gpio_output_en(LED1);
	gpio_output_en(LED2);
	gpio_output_en(LED3);
	gpio_output_en(LED4);
    /*
	* Button matrix table:
	* 			KEY3	KEY4
	* 	KEY1	SW2		SW3
	* 	KEY2	SW4		SW5
    */
	gpio_function_en(KEY3);
	gpio_output_en(KEY3);
	gpio_set_low_level(KEY3);

	gpio_function_en(KEY1);
	gpio_output_dis(KEY1);
	gpio_input_en(KEY1);
	gpio_set_up_down_res(KEY1, GPIO_PIN_PULLUP_10K);

	core_interrupt_enable();

#if (GPIO_MODE == GPIO_IRQ )
	 /****GPIO_IRQ POL_FALLING   Press SW2 to connect KEY1 and KEY3 to trigger an interrupt. **/
    //if disable gpio interrupt,choose disable gpio mask , use interface gpio_clr_irq_mask instead of gpio_irq_dis,if use gpio_irq_dis,may generate a false interrupt.
	gpio_set_irq(KEY1, INTR_FALLING_EDGE);	//When SW2 is pressed, the falling edge triggers the interrupt.
	plic_interrupt_enable(IRQ_GPIO);

#elif(GPIO_MODE == GPIO_IRQ_RISC0)
	/****GPIO_IRQ_risc0  POL_FALLING   Press SW2 to connect KEY1 and KEY3 to trigger an interrupt. **/
	//if disable gpio2risc0 interrupt,choose disable gpio2risc0 mask , use interface gpio_clr_irq_mask instead of gpio_gpio2risc0_irq_dis,if use gpio_gpio2risc0_irq_dis,may generate a false interrupt..
	gpio_set_gpio2risc0_irq(KEY1,INTR_FALLING_EDGE);	//When SW2 is pressed, the falling edge triggers the interrupt.
	plic_interrupt_enable(IRQ_GPIO2RISC0);

#elif(GPIO_MODE == GPIO_IRQ_RISC1)
	/****GPIO_IRQ_risc1  POL_FALLING   Press SW2 to connect KEY1 and KEY3 to trigger an interrupt. **/
	//if disable gpio2risc1 interrupt,choose disable gpio2risc1 mask , use interface gpio_clr_irq_mask instead of gpio_gpio2risc1_irq_dis,if use gpio_gpio2risc1_irq_dis,may generate a false interrupt.
	gpio_set_gpio2risc1_irq(KEY1,INTR_FALLING_EDGE);	//When SW2 is pressed, the falling edge triggers the interrupt.
	plic_interrupt_enable(IRQ_GPIO2RISC1);

#elif((GPIO_MODE == GPIO_SEL_IRQ_SRC)&&(MCU_CORE_B92))
	/*Note : the use method of 8 new GPIO irq source :
	 * First ,you can choose a gpio group,like gpio GPIO_GROUP_A,8 GPIO in all.
	 * Second,the 8 gpio you choose corresponding to the 8 gpio irq source respectively.for example,irq source0-GPIO_PA0,irq source1-GPIO-PA1......
	 * Attention:Once you choose a gpio-group,you can only use the gpio as irq_source in this group.And you should obey the rule of correspondence.
	 */
	gpio_set_src_irq_group(KEY1>>8);
	gpio_set_src_irq(KEY1,INTR_FALLING_EDGE);	//When SW2 is pressed, the falling edge triggers the interrupt.
	plic_interrupt_enable(IRQ_GPIO_SRC0 + BIT_LOW_BIT(KEY1&0x00ff));
#endif
#endif
#endif
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	gpio_toggle(LED1);
	delay_ms(200);
}

#endif

