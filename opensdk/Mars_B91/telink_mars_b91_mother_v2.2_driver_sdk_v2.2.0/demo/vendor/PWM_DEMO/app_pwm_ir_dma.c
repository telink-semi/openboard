/********************************************************************************************************
 * @file    app_pwm_ir_dma.c
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
#if(SET_PWM_MODE== PWM_IR_DMA)

#if(MCU_CORE_B91)
#define PWM_PIN		(PWM_PWM0_PB4)
#define PWM_ID		(get_pwmid(PWM_PIN))
#elif(MCU_CORE_B92)
#define PWM_ID		PWM0_ID
#define PWM_PIN		GPIO_FC_PB4
#define PWM_FUNC     PWM0
#endif

#define  DMA_CHN   DMA2


/*
 * pwm_ir_dma or pwm_chain_dma
 */
#define	 PWM_IR_FIFO_DMA	   1
#define	 PWM_CHAIN_DMA	       2
#define SET_PWM_DMA_MODE     PWM_IR_FIFO_DMA

#if (SET_PWM_DMA_MODE  == PWM_IR_FIFO_DMA)

	unsigned short IR_DMA_Buff[20]={0};

	unsigned short IRQ_IR_DMA_Buff[20]={0};

	volatile unsigned char irq_index=0;

	volatile unsigned char index1=0;

#elif(SET_PWM_DMA_MODE  == PWM_CHAIN_DMA)

	#define    CHAIN_BUFFER_SIZE   8

	volatile signed short CHAIN_DMA_Buff[2][CHAIN_BUFFER_SIZE>>1] __attribute__((aligned(4)));

	dma_chain_config_t tx_dma_list[2];

	volatile unsigned char index2=0;

	volatile unsigned char index3=0;

#endif


#if (SET_PWM_DMA_MODE  == PWM_IR_FIFO_DMA)
	_attribute_ram_code_sec_ void pwm_irq_handler(void)
{

	if(pwm_get_irq_status(FLD_PWM0_IR_DMA_FIFO_IRQ))
	{
	   pwm_clr_irq_status(FLD_PWM0_IR_DMA_FIFO_IRQ);

	   irq_index=0;

	   gpio_toggle(LED4);

	   IRQ_IR_DMA_Buff[irq_index]= pwm_cal_pwm0_ir_fifo_cfg_data(4, 0, 1);

	   IRQ_IR_DMA_Buff[++irq_index]= pwm_cal_pwm0_ir_fifo_cfg_data(4, 0, 1);

	   pwm_set_dma_buf(DMA_CHN,(unsigned int) IRQ_IR_DMA_Buff,4);

	   pwm_ir_dma_mode_start(DMA_CHN);

	}
	else
	{
		gpio_set_high_level(LED3);
	}


}
#endif



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

#if(MCU_CORE_B91)
        pwm_set_pin(PWM_PIN);
#elif(MCU_CORE_B92)
        pwm_set_pin(PWM_PIN,PWM_FUNC);
#endif

     pwm_set_clk((unsigned char) (sys_clk.pclk*1000*1000/PWM_PCLK_SPEED-1));

	 pwm_set_tcmp(PWM_ID,50 * CLOCK_PWM_CLOCK_1US);

	 pwm_set_tmax(PWM_ID,100 * CLOCK_PWM_CLOCK_1US);

	 pwm_set_pwm0_tcmp_and_tmax_shadow(200 * CLOCK_PWM_CLOCK_1US,100 * CLOCK_PWM_CLOCK_1US);

	 pwm_set_pwm0_mode(PWM_IR_DMA_FIFO_MODE);

	 pwm_set_dma_config(DMA_CHN);

#if (SET_PWM_DMA_MODE  == PWM_IR_FIFO_DMA)

	 IR_DMA_Buff[index1]= pwm_cal_pwm0_ir_fifo_cfg_data(5, 0, 1);

	 IR_DMA_Buff[++index1]= pwm_cal_pwm0_ir_fifo_cfg_data(4, 0, 1);

	 IR_DMA_Buff[++index1]=pwm_cal_pwm0_ir_fifo_cfg_data(6, 1, 1);

	 pwm_set_dma_buf(DMA_CHN, (unsigned int) IR_DMA_Buff ,6);

     pwm_set_irq_mask(FLD_PWM0_IR_DMA_FIFO_IRQ);

	 pwm_clr_irq_status(FLD_PWM0_IR_DMA_FIFO_IRQ);

	 core_interrupt_enable();

	 plic_interrupt_enable(IRQ16_PWM);

#elif(SET_PWM_DMA_MODE  == PWM_CHAIN_DMA)

     CHAIN_DMA_Buff[0][index2]= pwm_cal_pwm0_ir_fifo_cfg_data(5,  1, 1);

	 CHAIN_DMA_Buff[0][++index2]= pwm_cal_pwm0_ir_fifo_cfg_data(4, 1, 1);

	 CHAIN_DMA_Buff[0][++index2]= pwm_cal_pwm0_ir_fifo_cfg_data(6, 1, 1);

	 CHAIN_DMA_Buff[0][++index2]= pwm_cal_pwm0_ir_fifo_cfg_data(3, 1, 1);

	 CHAIN_DMA_Buff[1][index3]= pwm_cal_pwm0_ir_fifo_cfg_data(5,  0, 1);

	 CHAIN_DMA_Buff[1][++index3]= pwm_cal_pwm0_ir_fifo_cfg_data(4, 0, 1);

	 CHAIN_DMA_Buff[1][++index3]= pwm_cal_pwm0_ir_fifo_cfg_data(6, 0, 1);

	 CHAIN_DMA_Buff[1][++index3]= pwm_cal_pwm0_ir_fifo_cfg_data(3, 0, 1);

     pwm_set_dma_chain_llp(DMA_CHN,(unsigned short*)(&CHAIN_DMA_Buff[0]),CHAIN_BUFFER_SIZE,&tx_dma_list[0]);

	 pwm_set_tx_dma_add_list_element(DMA_CHN,&tx_dma_list[0],&tx_dma_list[1],(unsigned short*)(&CHAIN_DMA_Buff[1]),CHAIN_BUFFER_SIZE);

     pwm_set_tx_dma_add_list_element(DMA_CHN,&tx_dma_list[1],&tx_dma_list[0],(unsigned short*)(&CHAIN_DMA_Buff[0]),CHAIN_BUFFER_SIZE);

#endif

     pwm_ir_dma_mode_start(DMA_CHN);

}



void main_loop(void)
{
    delay_ms(500);

    gpio_toggle(LED2);
}

#endif



