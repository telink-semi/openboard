/********************************************************************************************************
 * @file    mic_app.c
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
#if(USB_DEMO_TYPE==USB_MICROPHONE)
#include "usb_default.h"
#include "application/usbstd/usb.h"
#define MIC_SAMPLING_RATE   (MIC_SAMPLE_RATE== 8000) ?  AUDIO_8K :((MIC_SAMPLE_RATE== 16000) ?  AUDIO_16K :(  (MIC_SAMPLE_RATE== 32000) ?  AUDIO_32K :( (MIC_SAMPLE_RATE== 48000) ? AUDIO_48K : AUDIO_16K) ) )

#define	MIC_BUFFER_SIZE			2048


unsigned short		iso_in_buff[MIC_BUFFER_SIZE];
#if(MCU_CORE_B92)
extern volatile unsigned int g_vbus_timer_turn_off_start_tick;
extern volatile unsigned int g_vbus_timer_turn_off_flag;
#endif
#if(MCU_CORE_B91)
#define AUDIO_LINE_IN            0
#define AUDIO_AMIC_IN            1
#define AUDIO_DMIC_IN            2

#define  AUDIO_IN_MODE          AUDIO_AMIC_IN
#define MIC_MONO_STEREO       ((MIC_CHANNEL_COUNT==1) ?  MONO_BIT_16 :STEREO_BIT_16 )
#define MIC_DMA_CHN             DMA2

volatile unsigned int		iso_in_w = 0;
volatile unsigned int  	     iso_in_r = 0;
unsigned int		num_iso_in = 0;
/**
 * @brief     This function serves to send data to USB. only adaptive mono 16bit
 * @param[in] audio_rate - audio rate. This value is matched with usb_default.h :MIC_SAMPLE_RATE.
 * @return    none.
 */

void audio_tx_data_to_usb(audio_sample_rate_e audio_rate)
{
	unsigned char length = 0;
	iso_in_w = ((audio_get_rx_dma_wptr (MIC_DMA_CHN) - (unsigned int)iso_in_buff) >> 1);
	 usbhw_reset_ep_ptr(USB_EDP_MIC);//reset pointer of Endpoint7's buf
	switch(audio_rate)
	{
		case 	AUDIO_8K:	length = 8* MIC_CHANNEL_COUNT;break;
		case	AUDIO_16K:	length = 16*MIC_CHANNEL_COUNT;break;
		case	AUDIO_32K:	length = 32*MIC_CHANNEL_COUNT;break;
		case	AUDIO_48K:	length = 48*MIC_CHANNEL_COUNT;break;
		default:			length = 16*MIC_CHANNEL_COUNT;break;
	}

	if (0 &&(iso_in_w - (iso_in_r&(MIC_BUFFER_SIZE-1)))< length)
	{
		for (unsigned char i=0; i<length*2; i++)
		{
			reg_usb_ep7_dat = 0;
		}
	}

	else
	{
		for (unsigned char i=0; i<length&& iso_in_r != iso_in_w ; i++)
		{
			short md = iso_in_buff[iso_in_r++ &(MIC_BUFFER_SIZE-1)];
			reg_usb_ep7_dat = md;
			reg_usb_ep7_dat = md >>8;
		}

	}
	 usbhw_data_ep_ack(USB_EDP_MIC);
}
/**
 * @brief		This function serves to handle  iso in usb endpoint interrupt ,interval is 1 ms
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_ void  usb_endpoint_irq_handler (void)
{
	if (usbhw_get_eps_irq() & FLD_USB_EDP7_IRQ)
	{

		usbhw_clr_eps_irq(FLD_USB_EDP7_IRQ);;	//clear interrupt flag of endpoint 7
		/////// get MIC input data ///////////////////////////////
		audio_tx_data_to_usb(MIC_SAMPLING_RATE);
		num_iso_in++;
		if ((num_iso_in & 0x7f) == 0)		gpio_toggle(LED1);
	}

}

void user_init(void)
{
#if(CHIP_VER_A0==CHIP_VER)
	audio_set_codec_supply(CODEC_2P8V);
#endif
	gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);

	reg_usb_ep6_buf_addr = 0x40;		// 192 max
	reg_usb_ep7_buf_addr = 0x20;		// 32
	reg_usb_ep8_buf_addr = 0x00;
	reg_usb_ep_max_size = (192 >> 3);
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
	//3.enable global interrupt
	core_interrupt_enable();
	plic_interrupt_enable(IRQ11_USB_ENDPOINT);		// enable usb endpoint interrupt
	usbhw_set_eps_irq_mask(FLD_USB_EDP7_IRQ);
#if(MCU_CORE_B91)
	usbhw_set_irq_mask(USB_IRQ_RESET_MASK|USB_IRQ_SUSPEND_MASK);
#endif
#if(AUDIO_IN_MODE==AUDIO_LINE_IN)
   // set in path digital and analog gain, must be set before audio_init();
	audio_set_codec_in_path_a_d_gain(CODEC_IN_D_GAIN_12_DB,CODEC_IN_A_GAIN_20_DB);
	audio_init(LINE_IN_TO_BUF ,MIC_SAMPLING_RATE,MIC_MONO_STEREO);
	audio_rx_dma_chain_init(MIC_DMA_CHN,(unsigned short*)&iso_in_buff,MIC_BUFFER_SIZE*2);
#elif(AUDIO_IN_MODE==AUDIO_DMIC_IN)
	audio_set_dmic_pin(DMIC_GROUPD_D4_DAT_D5_D6_CLK);
	// set in path digital gain,analog gain does not work, must be set before audio_init(),;
	audio_set_codec_in_path_a_d_gain(CODEC_IN_D_GAIN_12_DB,CODEC_IN_A_GAIN_0_DB);
	audio_init(DMIC_IN_TO_BUF ,MIC_SAMPLING_RATE,MIC_MONO_STEREO);
	audio_rx_dma_chain_init(MIC_DMA_CHN,(unsigned short*)&iso_in_buff,MIC_BUFFER_SIZE*2);
#elif(AUDIO_IN_MODE==AUDIO_AMIC_IN)
	// set in path digital and analog gain, must be set before audio_init();
	audio_set_codec_in_path_a_d_gain(CODEC_IN_D_GAIN_12_DB,CODEC_IN_A_GAIN_20_DB);
	audio_init(AMIC_IN_TO_BUF ,MIC_SAMPLING_RATE,MIC_MONO_STEREO);
	audio_rx_dma_chain_init(MIC_DMA_CHN,(unsigned short*)&iso_in_buff,MIC_BUFFER_SIZE*2);
#endif

}

void main_loop (void)
{
	usb_handle_irq();
}

#elif(MCU_CORE_B92)
#define AUDIO_LINE_IN            0
#define AUDIO_AMIC_IN            1
#define AUDIO_DMIC0_IN           2
#define DMIC0_DMIC1_IN           3
#define AUDIO_IN_MODE          AUDIO_DMIC0_IN


#if(AUDIO_IN_MODE ==DMIC0_DMIC1_IN)
#define TX_DATA_FOR_DMIC0        0 //The data of DMIC0 and DMIC1 are stored in one buff,only process DMIC0  data( the second 4bytes in the buff).
#define TX_DATA_FOR_DMIC1        1 //The data of DMIC0 and DMIC1 are stored in one buff,only process DMIC1 data( the first 4bytes in the buff).

#define USB_TX_MODE             TX_DATA_FOR_DMIC0
#endif


volatile unsigned int  	     iso_in_r = 0;
unsigned int		         num_iso_in = 0;
/**
 * @brief     This function serves to send data to USB. only adaptive mono 16bit
 * @param[in] audio_rate - audio rate. This value is matched with usb_default.h :MIC_SAMPLE_RATE.
 * @return    none.
 */
#if (USB_MODE==INT)
void audio_tx_data_to_usb(audio_sample_rate_e audio_rate)
{
	unsigned char length = 0;
	 usbhw_reset_ep_ptr(USB_EDP_MIC);//reset pointer of Endpoint7's buf
	switch(audio_rate)
	{
		case 	AUDIO_8K:	length = 8* MIC_CHANNEL_COUNT;break;
		case	AUDIO_16K:	length = 16*MIC_CHANNEL_COUNT;break;
		case	AUDIO_32K:	length = 32*MIC_CHANNEL_COUNT;break;
		case	AUDIO_48K:	length = 48*MIC_CHANNEL_COUNT;break;
		default:			length = 16*MIC_CHANNEL_COUNT;break;
	}

#if ((AUDIO_IN_MODE==DMIC0_DMIC1_IN)&&(USB_TX_MODE==TX_DATA_FOR_DMIC1))//process DMIC1 data
	for (unsigned char i=0; i<length ; i++)
		{
			short md=0;
			if(((i&0x01)==0))
			{
				if(iso_in_r==0)
				{
					iso_in_r=0;
				}
				else
				{
					iso_in_r=iso_in_r+3;
				}

			}
			else
			{
				iso_in_r=iso_in_r+1;
			}
			md = iso_in_buff[(iso_in_r) &(MIC_BUFFER_SIZE-1)];
			reg_usb_ep7_dat = md;
			reg_usb_ep7_dat = md >>8;
		}

#elif(AUDIO_IN_MODE==DMIC0_DMIC1_IN&&(USB_TX_MODE==TX_DATA_FOR_AMIC))//process AMIC data

		for (unsigned char i=0; i<length ; i++)
		{
			short md=0;
			if(((i&0x01)==0))
			{
				if(iso_in_r==0)
				{
					iso_in_r=2;
				}
				else
				{
					iso_in_r=iso_in_r+3;
				}

			}
			else
			{
				iso_in_r=iso_in_r+1;
			}
			md = iso_in_buff[(iso_in_r) &(MIC_BUFFER_SIZE-1)];

			reg_usb_ep7_dat = md;
			reg_usb_ep7_dat = md >>8;
		}



#else
	for (unsigned char i=0; i<length ; i++)
		{
			short md = iso_in_buff[iso_in_r++ &(MIC_BUFFER_SIZE-1)];
			reg_usb_ep7_dat = md;
			reg_usb_ep7_dat = md >>8;
		}

#endif
	 usbhw_data_ep_ack(USB_EDP_MIC);
}

/**
 * @brief		This function serves to handle  iso in usb endpoint interrupt ,interval is 1 ms
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_noinline_ void  usb_endpoint_irq_handler (void)
{
	if (usbhw_get_eps_irq() & FLD_USB_EDP7_IRQ)
	{

		usbhw_clr_eps_irq(FLD_USB_EDP7_IRQ);;	//clear interrupt flag of endpoint 7
		/////// get MIC input data ///////////////////////////////
		audio_tx_data_to_usb(MIC_SAMPLING_RATE);
		num_iso_in++;
		if ((num_iso_in & 0x7f) == 0)		gpio_toggle(LED2);
	}
}
#endif


#define    AUDIO_BUFF_SIZE    4096
volatile signed short AUDIO_BUFF[AUDIO_BUFF_SIZE>>1] __attribute__((aligned(4)));

void user_init(void)
{
    reg_usb_ep7_buf_addr = 0x00;//must to trigger it
	reg_usb_ep_max_size = (192>> 3);
	gpio_function_en(LED1);
    gpio_output_en(LED1);
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
#if (USB_MODE==INT)
#define FIFO_NUM          FIFO0
#define MIC_DMA_CHN       DMA2

	//3.enable global interrupt
	core_interrupt_enable();
	plic_interrupt_enable(IRQ11_USB_ENDPOINT);		// enable usb endpoint interrupt
	usbhw_set_eps_irq_mask(FLD_USB_EDP7_IRQ);

#if(AUDIO_IN_MODE==AUDIO_LINE_IN)
	audio_codec_stream0_input_t audio_codec_input =
	{
		.input_src = LINE_STREAM0_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};
	audio_codec_init();
	audio_codec_stream0_input_init(&audio_codec_input);
	audio_rx_dma_chain_init(audio_codec_input.fifo_num,audio_codec_input.dma_num,(unsigned short*)audio_codec_input.data_buf,audio_codec_input.data_buf_size);
	audio_rx_dma_en(audio_codec_input.dma_num);
#elif(AUDIO_IN_MODE==AUDIO_AMIC_IN)
	audio_codec_stream0_input_t audio_codec_input =
	{
		.input_src = AMIC_STREAM0_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};
    audio_codec_init();
    audio_codec_stream0_input_init(&audio_codec_input);
	audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_30_DB);
	audio_rx_dma_chain_init(audio_codec_input.fifo_num,audio_codec_input.dma_num,(unsigned short*)audio_codec_input.data_buf,audio_codec_input.data_buf_size);
	audio_rx_dma_en(audio_codec_input.dma_num);

#elif(AUDIO_IN_MODE==AUDIO_DMIC0_IN)
	audio_codec_stream0_input_t audio_codec_dmic0_input =
	{
		.input_src	 = DMIC_STREAM0_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO0,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};
		 /****dmic input ****/
	audio_codec_init();
		audio_set_stream0_dmic_pin(GPIO_FC_PD5,GPIO_FC_PD4,GPIO_FC_PD3);
	audio_codec_stream0_input_init(&audio_codec_dmic0_input);
	audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_30_DB);
	audio_rx_dma_chain_init(audio_codec_dmic0_input.fifo_num,audio_codec_dmic0_input.dma_num,(unsigned short*)audio_codec_dmic0_input.data_buf,audio_codec_dmic0_input.data_buf_size);
	audio_rx_dma_en(audio_codec_dmic0_input.dma_num);
#elif(AUDIO_IN_MODE==DMIC0_DMIC1_IN)
	audio_codec_stream0_input_t audio_codec_dmic0_input =
	{
		.input_src = DMIC_STREAM0_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,//must 16 bit
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};
	 audio_codec_stream1_input_t audio_codec_dmic1_input =
	 {
		.input_src	 = DMIC_STREAM0_STREAM1_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,//must 16 bit
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};

	audio_codec_init();
	audio_set_stream0_dmic_pin(GPIO_FC_PD5,GPIO_FC_PD4,GPIO_FC_PD3);
	audio_set_stream1_dmic_pin(GPIO_FC_PD2,GPIO_FC_PD7,GPIO_FC_PD6);
	audio_codec_stream0_input_init(&audio_codec_dmic0_input);
	audio_codec_stream1_input_init(&audio_codec_dmic1_input);
	audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_30_DB);
	audio_set_stream1_dig_gain(CODEC_IN_D_GAIN1_30_DB);
	audio_rx_dma_chain_init(audio_codec_dmic1_input.fifo_num,audio_codec_dmic1_input.dma_num,(unsigned short*)audio_codec_dmic1_input.data_buf,audio_codec_dmic1_input.data_buf_size);
	audio_rx_dma_en(MIC_DMA_CHN);

#endif
#elif (USB_MODE==AISO)
#define FIFO_NUM              FIFO0
#define MIC_DMA_CHN             DMA2
#define AISO_DMA_CHN            DMA3
#if(AUDIO_IN_MODE==AUDIO_LINE_IN)
	 audio_codec_stream0_input_t audio_codec_input =
	 {
		.input_src = LINE_STREAM0_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};
	audio_codec_init();
	audio_codec_stream0_input_init(&audio_codec_input);
	audio_rx_dma_chain_init(audio_codec_input.fifo_num,audio_codec_input.dma_num,(unsigned short*)audio_codec_input.data_buf,audio_codec_input.data_buf_size);
#elif(AUDIO_IN_MODE==AUDIO_AMIC_IN)
	 audio_codec_stream0_input_t audio_codec_input =
	 {
		.input_src = AMIC_STREAM0_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};
	audio_codec_init();
	audio_codec_stream0_input_init(&audio_codec_input);
	audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_30_DB);
	audio_rx_dma_chain_init(audio_codec_input.fifo_num,audio_codec_input.dma_num,(unsigned short*)audio_codec_input.data_buf,audio_codec_input.data_buf_size);
#elif(AUDIO_IN_MODE==AUDIO_DMIC0_IN)
	audio_codec_stream0_input_t audio_codec_dmic0_input =
	{
		.input_src	 = DMIC_STREAM0_STEREO,
		.sample_rate = MIC_SAMPLING_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = MIC_DMA_CHN,
		.data_buf = iso_in_buff,
		.data_buf_size = sizeof(iso_in_buff),
	};
	audio_codec_init();
	audio_set_stream0_dmic_pin(GPIO_FC_PD5,GPIO_FC_PD4,GPIO_FC_PD3);
	audio_codec_stream0_input_init(&audio_codec_dmic0_input);
	audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_30_DB);
	audio_rx_dma_chain_init(audio_codec_dmic0_input.fifo_num,audio_codec_dmic0_input.dma_num,(unsigned short*)audio_codec_dmic0_input.data_buf,audio_codec_dmic0_input.data_buf_size);
#endif
	audio_data_fifo_output_path_sel(FIFO_NUM,USB_AISO_OUT_FIFO);
	audio_tx_dma_chain_init(FIFO_NUM,AISO_DMA_CHN,(unsigned short*)iso_in_buff,MIC_BUFFER_SIZE*2);
	audio_rx_dma_en(MIC_DMA_CHN);
	audio_tx_dma_en(AISO_DMA_CHN);
#endif
}
unsigned int t1=0;
void main_loop (void)
{
	/**
	 * @attention   When using the vbus (not vbat) power supply, you must turn off the vbus timer,
	 *              otherwise the MCU will be reset after 8s.
	*/
#if(MCU_CORE_B92 && (POWER_SUPPLY_MODE == VBUS_POWER_SUPPLY))
	/**
     *When using the vbus (not vbat) power supply, the vbus detect status remains at 1. Conversely, it is 0.
     */
if(usb_get_vbus_detect_status())
{
	if(clock_time_exceed(g_vbus_timer_turn_off_start_tick, 100*1000) && (g_vbus_timer_turn_off_flag == 0))
	{
		/**
		 * wd_turn_off_vbus_timer() is used to turn off the 8s vbus timer.
		 * The vbus detect status will not be clear to 0.
		 */
		wd_turn_off_vbus_timer();
		g_vbus_timer_turn_off_flag = 1;
	}
}
else
{
	g_vbus_timer_turn_off_start_tick = stimer_get_tick();
	g_vbus_timer_turn_off_flag = 0;
}
#endif
	usb_handle_irq();

	if(clock_time_exceed(t1,500000))
		{
		t1 = stimer_get_tick()|1;
			 gpio_toggle(LED1);

		  }
}
#endif
#endif


