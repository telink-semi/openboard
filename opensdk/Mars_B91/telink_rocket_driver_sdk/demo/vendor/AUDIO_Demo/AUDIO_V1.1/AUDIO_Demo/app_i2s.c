/********************************************************************************************************
 * @file    app_i2s.c
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
#if ((AUDIO_MODE>DMA_IRQ_Test) && (AUDIO_MODE<=I2S_TO_EXT_CODEC_WM))
#include "ext_codec_wm/ext_codec_wm.h"
#include "app_sin_data.h"
#define    AUDIO_BUFF_SIZE  4096
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE>>1] __attribute__((aligned(4)));
signed short AUDIO_BUFF1[AUDIO_BUFF_SIZE>>1] __attribute__((aligned(4)));

/**
 *                                          i2s_clk_config[2]   i2s_clk_config[3]-->lrclk_adc(sampling rate)
 *                                                  ||                  ||
 *  pll(192M default)------->div---->i2s_clk--->2 * div(div=0,bypass)--->blck----->div
 *                           ||                                         ||
 *           i2s_clk_config[0]/i2s_clk_config[1]                 i2s_clk_config[4]-->lrclk_dac(sampling rate)
 * For example:sampling rate=16K, i2s_clk_config[5]={ 8,125,6,64,64}, sampling rate=192M*(8/125)/(2*6)/64=16K
 *
 */
unsigned short audio_i2s_8k_config[5] 	= {8,125,12,64,64};//AUDIO_RATE_EQUAL	8000
unsigned short audio_i2s_16k_config[5][5] =
{
	{ 8, 	125,	6,	64, 64},		//AUDIO_RATE_EQUAL	16000  i2s_clk=12.288M
	{23,	154,	14,	64, 64},		//AUDIO_RATE_GT_L0	16001.86
	{21,	164,	12,	64,	64},		//AUDIO_RATE_GT_L1	16006.1
	{33,	221,	14,	64, 64},		//AUDIO_RATE_LT_L0	15998.71
	{27,	211,	12,	64, 64},		//AUDIO_RATE_LT_L0	15995.26
};

unsigned short audio_i2s_32k_config[5]  =	 {8, 125,3,  64, 64};//32000

unsigned short  audio_i2s_48k_config[5][5] =
{
	{2,	    125,	0,	64,	64},		//AUDIO_RATE_EQUAL	48000   192M*(1)/(2*6)/64=48K
	{69,	154,	14,	64, 64},		//AUDIO_RATE_GT_L0	48005
	{70,	243,	9,	64,	64},		//AUDIO_RATE_GT_L1	48010
	{91,	237,	12,	64, 64},		//AUDIO_RATE_LT_L0	47995
	{82,	233,	11,	64, 64},		//AUDIO_RATE_LT_L1	47990
};

unsigned short  audio_i2s_44k1_config[5][5] =
{
    {76, 	235,	11,	64, 64},		//AUDIO_RATE_EQUAL	44100
	{101, 	229,	15,	64, 64},		//AUDIO_RATE_GT_L0	44105
	{1,		17,		2,	64, 64},		//AUDIO_RATE_GT_L1	44117
	{60,	157,	13,	64, 64},		//AUDIO_RATE_LT_L0	44096
	{81, 	212,	13,	64, 64},		//AUDIO_RATE_LT_L1	44085
};


void user_init(void)
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);
#if (AUDIO_MODE== I2S_TO_EXT_CODEC_WM)
	i2s_pin_config_t i2s_pin_config =
	{
		.bclk_pin   	= GPIO_FC_PA3,
		.adc_lr_clk_pin = GPIO_FC_PA2,
		.adc_dat_pin    = GPIO_FC_PB5,
		.dac_lr_clk_pin = GPIO_FC_PC0,
		.dac_dat_pin    = GPIO_FC_PB1,
	};
	audio_i2s_config_t audio_i2s_config =
	{
		.i2s_select = I2S0,
		.i2s_mode = I2S_I2S_MODE,
		.pin_config = &i2s_pin_config,
		.data_width = I2S_BIT_16_DATA,
		.master_slave_mode = I2S_AS_MASTER_EN,
		.sample_rate = audio_i2s_32k_config,
	};
	audio_i2s_input_output_t audio_i2s_input =
	{
		.i2s_select = audio_i2s_config.i2s_select,
		.data_width = audio_i2s_config.data_width,
		.i2s_ch_sel = I2S_STEREO,
		.fifo_chn = FIFO0,
		.dma_num = DMA0,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};
	audio_i2s_input_output_t audio_i2s_output =
	{
		.i2s_select = audio_i2s_config.i2s_select,
		.data_width = audio_i2s_config.data_width,
		.i2s_ch_sel = I2S_STEREO,
		.fifo_chn = FIFO0,
		.dma_num = DMA1,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};
	audio_power_on();
	audio_i2c_init_wm(GPIO_FC_PC5,GPIO_FC_PB2,(unsigned char)(sys_clk.pclk*1000*1000/(4*200000)));//set i2c frequency 200K
    audio_i2s_set_mclk(GPIO_FC_PA4,1,16);//only set i2s as master mclk=192M*(1/16)=12M
    /****i2s config init ****/
	audio_i2s_config_init(&audio_i2s_config);
	/****i2s input init ****/
	audio_i2s_input_init(&audio_i2s_input);
	/****i2s output init ****/
    audio_i2s_output_init(&audio_i2s_output);
    /****rx tx dma init ****/
    audio_rx_dma_chain_init(audio_i2s_input.fifo_chn,audio_i2s_input.dma_num,(unsigned short*)audio_i2s_input.data_buf,audio_i2s_input.data_buf_size);
    audio_tx_dma_chain_init(audio_i2s_output.fifo_chn,audio_i2s_output.dma_num,(unsigned short*)audio_i2s_output.data_buf,audio_i2s_output.data_buf_size);
    /****i2s starts run****/
	audio_rx_dma_en(audio_i2s_input.dma_num);
	audio_tx_dma_en(audio_i2s_output.dma_num);

#elif(AUDIO_MODE== TWO_I2S_ALIGN)
	/*Note:
	  1. I2S align mode function description:
	     (1) When the sys_timer count to the set threshold ticks, I2S0 and I2S1 generate the same phase clock(LR_CLK) so as to achieve phase alignment .
	     (2) I2S1 share one FIFO1 thus data alignment is achieved, data format reference:data format to audio_set_i2s_align_data_width().
	  2. Initialization order requirements for align mode:must call the audio_set_i2s_align_en() to enable align mode function first, and then initialize the relevant configuration of IIS,
	     step(1) Must call the audio_set_i2s_align_en() first ,and set data format for align mode .
	     step(2) Configure the initialization of IIS
	     step(3) Call the audio_set_i2s_align_mask_dis() to disable the align mask
	     step(4) Set sys_timer threshold ticks
	     step(5) Call the audio_set_i2s_align_mask_en() to enable the align mask,
	 *3: Restriction: The selected FIFO can only be FIFO1, and the input and output paths can only be I2S1.
	 */
	   i2s_pin_config_t i2s0_pin_config =
	   {
			.bclk_pin   	= GPIO_FC_PA3,
			.adc_lr_clk_pin = GPIO_FC_PA2,
			.adc_dat_pin    = GPIO_FC_PB5,
			.dac_lr_clk_pin = GPIO_FC_PC0,
			.dac_dat_pin    = GPIO_FC_PB1,
		};
	   i2s_pin_config_t i2s1_pin_config =
	   {
	   		.bclk_pin   	= GPIO_FC_PA1,
	   		.adc_lr_clk_pin = GPIO_FC_PB4,
			.dac_dat_pin    = GPIO_FC_PB6,
	   		.dac_lr_clk_pin = GPIO_FC_PB7,
	   		.adc_dat_pin    = GPIO_FC_PA0,
	   		};
	   audio_i2s_config_t audio_i2s0_config =
	   {
			.i2s_select = I2S0,
			.i2s_mode = I2S_I2S_MODE,
			.pin_config = &i2s0_pin_config,
			.data_width = I2S_BIT_16_DATA,
			.master_slave_mode = I2S_AS_MASTER_EN,
			.sample_rate = audio_i2s_32k_config,
		};
	   audio_i2s_config_t audio_i2s1_config =
	   {
			.i2s_select = I2S1,
			.i2s_mode = I2S_I2S_MODE,
			.pin_config = &i2s1_pin_config,
			.data_width = I2S_BIT_16_DATA,
			.master_slave_mode = I2S_AS_MASTER_EN,
			.sample_rate = audio_i2s_32k_config,
		};

		audio_power_on();
		/***align mode initialization***/
		//step(1)
		audio_set_i2s_align_en();
		audio_set_i2s_align_data_width(I2S_ALIGN_BIT_16_DATA);
		//step(2)
	    audio_i2c_init_wm(GPIO_FC_PC5,GPIO_FC_PB2,(unsigned char)(sys_clk.pclk*1000*1000/(4*200000)));//set i2c frequency 200K
	    audio_i2s_set_mclk(GPIO_FC_PA4,1,16);//only set i2s as master mclk=192M*(1/16)=12M
	    /****i2s config init ****/
		audio_i2s_config_init(&audio_i2s0_config);
		audio_i2s_config_init(&audio_i2s1_config);
		/****i2s input and output path init ****/
		audio_data_fifo_input_path_sel(FIFO1,I2S1_IN_FIFO);  //must FIFO1 and I2S1
		audio_data_fifo_output_path_sel(FIFO1,I2S1_OUT_FIFO);//must FIFO1 and I2S1
		/****rx tx dma init ****/
		audio_rx_dma_chain_init(FIFO1,DMA0,(unsigned short*)AUDIO_BUFF,sizeof(AUDIO_BUFF));//must  FIFO1
		audio_tx_dma_chain_init(FIFO1,DMA1,(unsigned short*)AUDIO_BUFF,sizeof(AUDIO_BUFF));//must  FIFO1
		/****i2s starts run****/
		audio_rx_dma_en(DMA0);
		audio_tx_dma_en(DMA1);
		//step(3)
		audio_set_i2s_align_mask_dis();
		//step(4)
		audio_set_i2s_align_th_tick(stimer_get_tick());
		//step(5)
		audio_set_i2s_align_mask_en();
#elif(AUDIO_MODE== I2S_OUTPUT_DOUBLE_BUFF)
		__attribute__((aligned(4))) signed short buff_L[0x10] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0x0BB,0xCC,0xDD,0xEE,0xFF};

		__attribute__((aligned(4))) signed short buff_R[0x10] = {0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00};
		i2s_pin_config_t i2s0_pin_config =
		{
			.bclk_pin   	= GPIO_FC_PA3,
			.adc_lr_clk_pin = GPIO_FC_PA2,
			.adc_dat_pin    = GPIO_FC_PB5,
			.dac_lr_clk_pin = GPIO_FC_PC0,
			.dac_dat_pin    = GPIO_FC_PB1,
		};
		i2s_pin_config_t i2s1_pin_config =
		{
			.bclk_pin   	= GPIO_FC_PA1,
			.adc_lr_clk_pin = GPIO_FC_PB4,
			.adc_dat_pin    = GPIO_FC_PA0,
			.dac_lr_clk_pin = GPIO_FC_PB7,
			.dac_dat_pin    = GPIO_FC_PB6,
		};

		audio_i2s_config_t audio_i2s0_config =
		{
			.i2s_select = I2S0,
			.pin_config = &i2s0_pin_config,
			.data_width = I2S_BIT_16_DATA,
			.i2s_mode = I2S_I2S_MODE,
			.master_slave_mode = I2S_AS_MASTER_EN,
			.sample_rate = audio_i2s_32k_config,
		};

		audio_i2s_input_output_t audio_i2s0_output0 =
		{
			.i2s_select = audio_i2s0_config.i2s_select,
			.data_width = I2S_BIT_16_DATA_FIFO0_AND_FIFO1,
			.i2s_ch_sel = I2S_STEREO,
			.fifo_chn = FIFO0,
			.dma_num = DMA0,
			.data_buf = buff_L,
			.data_buf_size = sizeof(buff_L),
		};
		audio_i2s_input_output_t audio_i2s0_output1 =
		{
			.i2s_select = audio_i2s0_config.i2s_select,
			.data_width = I2S_BIT_16_DATA_FIFO0_AND_FIFO1,
			.i2s_ch_sel = I2S_STEREO,
			.fifo_chn = FIFO1,
			.dma_num = DMA1,
			.data_buf = buff_R,
			.data_buf_size = sizeof(buff_R),
		};

		audio_i2s_config_t audio_i2s1_config =
		{
			.i2s_select = I2S1,
			.i2s_mode = I2S_I2S_MODE,
			.pin_config = &i2s1_pin_config,
			.data_width = I2S_BIT_16_DATA,
			.master_slave_mode = I2S_AS_MASTER_EN,
			.sample_rate = audio_i2s_32k_config,
		};
		audio_i2s_input_output_t audio_i2s1_output0 =
		{
			.i2s_select = audio_i2s1_config.i2s_select,
			.data_width = I2S_BIT_16_DATA_FIFO0_AND_FIFO1,
			.i2s_ch_sel = I2S_STEREO,
			.fifo_chn = FIFO0,
			.dma_num = DMA2,
			.data_buf = buff_L,
			.data_buf_size = sizeof(buff_L),
		};
		audio_i2s_input_output_t audio_i2s1_output1 =
		{
			.i2s_select = audio_i2s1_config.i2s_select,
			.data_width = I2S_BIT_16_DATA_FIFO0_AND_FIFO1,
			.i2s_ch_sel = I2S_STEREO,
			.fifo_chn = FIFO1,
			.dma_num = DMA3,
			.data_buf = buff_R,
			.data_buf_size = sizeof(buff_R),
		};

		audio_power_on();
		/****i2s config init ****/
		audio_i2s_config_init(&audio_i2s0_config);
		audio_i2s_config_init(&audio_i2s1_config);
		/****i2s output init ****/
		audio_i2s_output_init(&audio_i2s0_output0);
		audio_i2s_output_init(&audio_i2s0_output1);
		audio_i2s_output_init(&audio_i2s1_output0);
		audio_i2s_output_init(&audio_i2s1_output1);
		/****tx dma init ****/
		audio_tx_dma_chain_init(audio_i2s0_output0.fifo_chn,audio_i2s0_output0.dma_num,(unsigned short*)audio_i2s0_output0.data_buf,audio_i2s0_output0.data_buf_size);
		audio_tx_dma_chain_init(audio_i2s0_output1.fifo_chn,audio_i2s0_output1.dma_num,(unsigned short*)audio_i2s0_output1.data_buf,audio_i2s0_output1.data_buf_size);
		audio_tx_dma_chain_init(audio_i2s1_output0.fifo_chn,audio_i2s1_output0.dma_num,(unsigned short*)audio_i2s1_output0.data_buf,audio_i2s1_output0.data_buf_size);
		audio_tx_dma_chain_init(audio_i2s1_output1.fifo_chn,audio_i2s1_output1.dma_num,(unsigned short*)audio_i2s1_output1.data_buf,audio_i2s1_output1.data_buf_size);
		/****i2s starts run****/
		audio_tx_dma_en(audio_i2s0_output0.dma_num);
		audio_tx_dma_en(audio_i2s0_output1.dma_num);
		audio_tx_dma_en(audio_i2s1_output0.dma_num);
		audio_tx_dma_en(audio_i2s1_output1.dma_num);
#elif(AUDIO_MODE== AUDIO_FIFO_IRQ_Test)
#define FIFO0_NUM            FIFO0
#define FIFO1_NUM            FIFO1

#define RX0_DMA_CHN          DMA0
#define TX0_DMA_CHN          DMA1
#define RX1_DMA_CHN          DMA2
#define TX1_DMA_CHN          DMA3
		i2s_pin_config_t i2s0_pin_config =
		{
			.bclk_pin   	= GPIO_FC_PA3,
			.adc_lr_clk_pin = GPIO_FC_PA2,
			.adc_dat_pin    = GPIO_FC_PB5,
			.dac_lr_clk_pin = GPIO_FC_PC0,
			.dac_dat_pin    = GPIO_FC_PB1,
		};
		i2s_pin_config_t i2s1_pin_config =
		{
			.bclk_pin   	= GPIO_FC_PA1,
			.adc_lr_clk_pin = GPIO_FC_PB4,
			.adc_dat_pin    = GPIO_FC_PA0,
			.dac_lr_clk_pin = GPIO_FC_PB7,
			.dac_dat_pin    = GPIO_FC_PB6,
		};
		audio_i2s_config_t audio_i2s0_config =
		{
			.i2s_select = I2S0,
			.i2s_mode = I2S_I2S_MODE,
			.data_width = I2S_BIT_16_DATA,
			.pin_config = &i2s0_pin_config,
			.master_slave_mode = I2S_AS_MASTER_EN,
			.sample_rate = audio_i2s_32k_config,
		};
		audio_i2s_config_t audio_i2s1_config =
		{
			.i2s_select = I2S1,
			.i2s_mode = I2S_I2S_MODE,
			.pin_config = &i2s1_pin_config,
			.data_width = I2S_BIT_16_DATA,
			.master_slave_mode = I2S_AS_MASTER_EN,
			.sample_rate = audio_i2s_32k_config,
		};

		audio_i2s_input_output_t audio_i2s0_input = {
			.i2s_select = audio_i2s0_config.i2s_select,
			.data_width = audio_i2s0_config.data_width,
			.i2s_ch_sel = I2S_MONO,
			.fifo_chn = FIFO0_NUM,
			.dma_num = RX0_DMA_CHN,
			.data_buf = AUDIO_BUFF,
			.data_buf_size = sizeof(AUDIO_BUFF),
		};
		audio_i2s_input_output_t audio_i2s0_output = {
			.i2s_select = audio_i2s0_config.i2s_select,
			.data_width = audio_i2s0_config.data_width,
			.i2s_ch_sel = I2S_MONO,
			.fifo_chn = FIFO0_NUM,
			.dma_num = TX0_DMA_CHN,
			.data_buf = AUDIO_BUFF,
			.data_buf_size = sizeof(AUDIO_BUFF),
		};
		audio_i2s_input_output_t audio_i2s1_input =
		{
			.i2s_select = audio_i2s1_config.i2s_select,
			.data_width = audio_i2s1_config.data_width,
			.i2s_ch_sel = I2S_MONO,
			.fifo_chn = FIFO1_NUM,
			.dma_num = RX1_DMA_CHN,
			.data_buf = AUDIO_BUFF1,
			.data_buf_size = sizeof(AUDIO_BUFF1),
		};
		audio_i2s_input_output_t audio_i2s1_output =
		{
			.i2s_select = audio_i2s1_config.i2s_select,
			.data_width = audio_i2s1_config.data_width,
			.i2s_ch_sel = I2S_MONO,
			.fifo_chn = FIFO1_NUM,
			.dma_num = TX1_DMA_CHN,
			.data_buf = AUDIO_BUFF1,
			.data_buf_size = sizeof(AUDIO_BUFF1),
		};

		audio_power_on();
		audio_i2c_init_wm(GPIO_FC_PC5,GPIO_FC_PB2,(unsigned char)(sys_clk.pclk*1000*1000/(4*200000)));//set i2c frequency 200K
		audio_i2s_set_mclk(GPIO_FC_PA4,1,16);//only set i2s as master mclk=192M*(1/16)=12M
		/****i2s config init ****/
		audio_i2s_config_init(&audio_i2s0_config);
		audio_i2s_config_init(&audio_i2s1_config);
		/****i2s input init ****/
		audio_i2s_input_init(&audio_i2s0_input);
		audio_i2s_input_init(&audio_i2s1_input);
		/****i2s output init ****/
		audio_i2s_output_init(&audio_i2s0_output);
		audio_i2s_output_init(&audio_i2s1_output);
		/****i2s fifo trigger threshold  init ****/
		audio_set_rx_fifo_threshold(FIFO0_NUM,AUDIO_BUFF_SIZE/2);
		audio_set_tx_fifo_threshold(FIFO0_NUM,AUDIO_BUFF_SIZE/2);
		audio_set_rx_fifo_threshold(FIFO1_NUM,AUDIO_BUFF_SIZE/2);
		audio_set_tx_fifo_threshold(FIFO1_NUM,AUDIO_BUFF_SIZE/2);
		/****i2s interrupt init ****/
		core_interrupt_enable();
		plic_interrupt_enable(IRQ_DFIFO);
		audio_set_irq_mask(AUDIO_RX_FIFO0_IRQ|AUDIO_TX_FIFO0_IRQ|AUDIO_RX_FIFO1_IRQ|AUDIO_TX_FIFO1_IRQ);
		/****rx tx dma init ****/
		audio_rx_dma_chain_init(audio_i2s0_input.fifo_chn,audio_i2s0_input.dma_num,(unsigned short*)audio_i2s0_input.data_buf,audio_i2s0_input.data_buf_size);
		audio_tx_dma_chain_init(audio_i2s0_output.fifo_chn,audio_i2s0_output.dma_num,(unsigned short*)audio_i2s0_output.data_buf,audio_i2s0_output.data_buf_size);
		audio_rx_dma_chain_init(audio_i2s1_input.fifo_chn,audio_i2s1_input.dma_num,(unsigned short*)audio_i2s1_input.data_buf,audio_i2s1_input.data_buf_size);
		audio_tx_dma_chain_init(audio_i2s1_output.fifo_chn,audio_i2s1_output.dma_num,(unsigned short*)audio_i2s1_output.data_buf,audio_i2s1_output.data_buf_size);
		/****i2s starts run****/
		audio_rx_dma_en(audio_i2s0_input.dma_num);
		audio_tx_dma_en(audio_i2s0_output.dma_num);
		audio_rx_dma_en(audio_i2s1_input.dma_num);
		audio_tx_dma_en(audio_i2s1_output.dma_num);
#endif

}

void main_loop (void)
{
	gpio_toggle(LED1);
	delay_ms(500);
}

#if(AUDIO_MODE==AUDIO_FIFO_IRQ_Test)
volatile unsigned int irq_cnt[4];
volatile unsigned int  fifo_dma_ptr_test[8];

_attribute_ram_code_sec_ void audio_irq_handler(void)
{
	if(audio_get_irq_status(AUDIO_RX_FIFO0_IRQ))//rx0
	{
		audio_clr_irq_status(AUDIO_RX_FIFO0_IRQ);
		fifo_dma_ptr_test[0]=audio_get_rx_dma_wptr(RX0_DMA_CHN)-(unsigned int )AUDIO_BUFF;
		fifo_dma_ptr_test[1]=audio_get_rx_wptr(FIFO0_NUM);// =fifo_dma_ptr_test[0]=fifo_dma_ptr_test[1]=(AUDIO_BUFF_SIZE/2)+4
		irq_cnt[0]++;

	}
	if(audio_get_irq_status(AUDIO_TX_FIFO0_IRQ))//tx0
	{
		audio_clr_irq_status(AUDIO_TX_FIFO0_IRQ);
		fifo_dma_ptr_test[2]=audio_get_tx_dma_rptr(TX0_DMA_CHN)-(unsigned int )AUDIO_BUFF;
		fifo_dma_ptr_test[3]=audio_get_tx_rptr(FIFO0_NUM);// =(AUDIO_BUFF_SIZE/2)+4
		irq_cnt[1]++;
	}
	if(audio_get_irq_status(AUDIO_RX_FIFO1_IRQ))//rx1
	{
		audio_clr_irq_status(AUDIO_RX_FIFO1_IRQ);
		fifo_dma_ptr_test[4]=audio_get_rx_dma_wptr(RX1_DMA_CHN)-(unsigned int )AUDIO_BUFF1;
		fifo_dma_ptr_test[5]=audio_get_rx_wptr(FIFO1_NUM);// =(AUDIO_BUFF_SIZE/2)+4
		irq_cnt[2]++;
	}
	if(audio_get_irq_status(AUDIO_TX_FIFO1_IRQ))//tx1
	{
		audio_clr_irq_status(AUDIO_TX_FIFO1_IRQ);
		fifo_dma_ptr_test[6]=audio_get_tx_dma_rptr(TX1_DMA_CHN)-(unsigned int )AUDIO_BUFF1;
		fifo_dma_ptr_test[7]=audio_get_tx_rptr(FIFO1_NUM);// =(AUDIO_BUFF_SIZE/2)+4
		irq_cnt[3]++;
	}

}
PLIC_ISR_REGISTER(audio_irq_handler, IRQ_DFIFO)
#endif
#endif
