#include "../app_config.h"
#include "ws2812b.h"
#include "serial_port.h"

unsigned short IRQ_IR_DMA_Buff[24]={0};
volatile unsigned int pwm_irq_index = 0; // The first time only WS2812B will be output NUM-1 24bit, but initially outputted 24bit
unsigned char g_grb[3] = {255,0,0x00};

void set_grb(unsigned char * grb)
{
	IRQ_IR_DMA_Buff[0] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X80 == (grb[0]&0X80), 1);
	IRQ_IR_DMA_Buff[1] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X40 == (grb[0]&0X40), 1);
	IRQ_IR_DMA_Buff[2] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X20 == (grb[0]&0X20), 1);
	IRQ_IR_DMA_Buff[3] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X10 == (grb[0]&0X10), 1);
	IRQ_IR_DMA_Buff[4] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X08 == (grb[0]&0X08), 1);
	IRQ_IR_DMA_Buff[5] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X04 == (grb[0]&0X04), 1);
	IRQ_IR_DMA_Buff[6] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X02 == (grb[0]&0X02), 1);
	IRQ_IR_DMA_Buff[7] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X01 == (grb[0]&0X01), 1);
	IRQ_IR_DMA_Buff[8] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X80 == (grb[1]&0X80), 1);
	IRQ_IR_DMA_Buff[9] = pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X40 == (grb[1]&0X40), 1);
	IRQ_IR_DMA_Buff[10]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X20 == (grb[1]&0X20), 1);
	IRQ_IR_DMA_Buff[11]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X10 == (grb[1]&0X10), 1);
	IRQ_IR_DMA_Buff[12]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X08 == (grb[1]&0X08), 1);
	IRQ_IR_DMA_Buff[13]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X04 == (grb[1]&0X04), 1);
	IRQ_IR_DMA_Buff[14]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X02 == (grb[1]&0X02), 1);
	IRQ_IR_DMA_Buff[15]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X01 == (grb[1]&0X01), 1);
	IRQ_IR_DMA_Buff[16]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X80 == (grb[2]&0X80), 1);
	IRQ_IR_DMA_Buff[17]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X40 == (grb[2]&0X40), 1);
	IRQ_IR_DMA_Buff[18]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X20 == (grb[2]&0X20), 1);
	IRQ_IR_DMA_Buff[19]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X10 == (grb[2]&0X10), 1);
	IRQ_IR_DMA_Buff[20]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X08 == (grb[2]&0X08), 1);
	IRQ_IR_DMA_Buff[21]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X04 == (grb[2]&0X04), 1);
	IRQ_IR_DMA_Buff[22]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X02 == (grb[2]&0X02), 1);
	IRQ_IR_DMA_Buff[23]= pwm_cal_pwm0_ir_fifo_cfg_data(1, 0X01 == (grb[2]&0X01), 1);
}

_attribute_ram_code_sec_ void pwm_irq_handler(void)
{
	if(pwm_get_irq_status(FLD_PWM0_IR_DMA_FIFO_IRQ)){
		pwm_clr_irq_status(FLD_PWM0_IR_DMA_FIFO_IRQ);
		pwm_irq_index++;
		if(pwm_irq_index != WS2812B_NUM){
		   pwm_set_dma_buf(DMA_CHN,(unsigned int) IRQ_IR_DMA_Buff,48);
		   pwm_ir_dma_mode_start(DMA_CHN);
		}
	}
}

void ws2812b_init(void)
{
	pwm_set_pin(PWM_PIN);
	pwm_set_clk((unsigned char) (sys_clk.pclk*1000*1000/PWM_PCLK_SPEED-1));
	pwm_set_tcmp(PWM_ID,5);
	pwm_set_tmax(PWM_ID,15);
	pwm_set_pwm0_tcmp_and_tmax_shadow(15,10);
	pwm_set_pwm0_mode(PWM_IR_DMA_FIFO_MODE);
	pwm_set_dma_config(DMA_CHN);
	set_grb(g_grb);
    pwm_set_dma_buf(DMA_CHN,(unsigned int) IRQ_IR_DMA_Buff,48);
	pwm_set_irq_mask(FLD_PWM0_IR_DMA_FIFO_IRQ);
	pwm_clr_irq_status(FLD_PWM0_IR_DMA_FIFO_IRQ);
	core_interrupt_enable();
	plic_interrupt_enable(IRQ16_PWM);
	pwm_ir_dma_mode_start(DMA_CHN);
}

void ws2812b_test(void)
{
	if(pwm_irq_index == WS2812B_NUM){
//		g_grb[0] ^= g_grb[1];
//		g_grb[1] ^= g_grb[0];
//		g_grb[0] ^= g_grb[1];
		g_grb[0]++;
		g_grb[1]--;
		g_grb[2]=g_grb[0]>125?g_grb[0]:g_grb[1];
		pwm_irq_index = 0;
		gpio_output_en(PWM_PIN);
		gpio_set_low_level(PWM_PIN);
		gpio_function_en(PWM_PIN);
		delay_us(300);
		pwm_set_pin(PWM_PIN);
		gpio_function_dis(PWM_PIN);
		set_grb(g_grb);
		pwm_set_dma_buf(DMA_CHN,(unsigned int) IRQ_IR_DMA_Buff,48);
		pwm_ir_dma_mode_start(DMA_CHN);
	}
}


