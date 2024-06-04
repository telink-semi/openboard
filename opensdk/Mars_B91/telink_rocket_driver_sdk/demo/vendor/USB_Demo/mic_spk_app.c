/********************************************************************************************************
 * @file    mic_spk_app.c
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
#if(USB_DEMO_TYPE==USB_MIC_SPEAKER)
#include "usb_default.h"
#include "application/usbstd/usb.h"


#define MIC_DMA_RX_CHN          DMA0
#define SPK_DMA_TX_CHN          DMA3

#define	MIC_BUFFER_SIZE			4096
#define	SPK_BUFFER_SIZE			4096

#define MIC_SAMPLING_RATE   (MIC_SAMPLE_RATE== 8000) ?  AUDIO_8K :((MIC_SAMPLE_RATE== 16000) ?  AUDIO_16K :(  (MIC_SAMPLE_RATE== 32000) ?  AUDIO_32K :( (MIC_SAMPLE_RATE== 48000) ? AUDIO_48K : AUDIO_16K) ) )
#define SPK_SAMPLING_RATE   (SPEAKER_SAMPLE_RATE== 8000) ?  AUDIO_8K :((SPEAKER_SAMPLE_RATE== 16000) ?  AUDIO_16K :(  (SPEAKER_SAMPLE_RATE== 32000) ?  AUDIO_32K :( (SPEAKER_SAMPLE_RATE== 48000) ? AUDIO_48K : AUDIO_16K) ) )

short iso_in_buff[MIC_BUFFER_SIZE];
short iso_out_buff[SPK_BUFFER_SIZE];
#if(MCU_CORE_B92)
extern volatile unsigned int g_vbus_timer_turn_off_start_tick;
extern volatile unsigned int g_vbus_timer_turn_off_flag;
#endif
int		iso_in_w = 0;
int		iso_in_r = 0;

int		iso_out_w = 0;
int		iso_out_r = 0;

/**
 * @brief     This function serves to send data to USB. only adaptive mono 16bit
 * @param[in] audio_rate - audio rate. This value is matched with usb_default.h :MIC_SAMPLE_RATE.
 * @return    none.
*/

void audio_tx_data_to_usb(audio_sample_rate_e audio_rate)
{
	unsigned char length = 0;
	iso_in_w = ((audio_get_rx_dma_wptr(MIC_DMA_RX_CHN) - (unsigned int)iso_in_buff) >> 1);
	 usbhw_reset_ep_ptr(USB_EDP_MIC);//reset pointer of Endpoint7's buf

	switch(audio_rate)
	{
		case 	AUDIO_8K:	length = 8* MIC_CHANNEL_COUNT;break;
		case	AUDIO_16K:	length = 16*MIC_CHANNEL_COUNT;break;
		case	AUDIO_32K:	length = 32*MIC_CHANNEL_COUNT;break;
		case	AUDIO_48K:	length = 48*MIC_CHANNEL_COUNT;break;
		default:			length = 16*MIC_CHANNEL_COUNT;break;
	}

		for (unsigned char i=0; i<length&& iso_in_r != iso_in_w ; i++)
		{
			short md = iso_in_buff[iso_in_r++ &(MIC_BUFFER_SIZE-1)];
			reg_usb_ep7_dat = md;
			reg_usb_ep7_dat = md >>8;
		}
   usbhw_data_ep_ack(USB_EDP_MIC);

}




/**
 * @brief     This function servers to set USB Input.
 * @param[in] none
 * @return    none.
 */
void  audio_rx_data_from_usb (void)
{
	unsigned char len = reg_usb_ep_ptr(USB_EDP_SPEAKER);
	usbhw_reset_ep_ptr(USB_EDP_SPEAKER);
	for (unsigned int i=0; i<len; i+=4)
	{
		signed short d0 = reg_usb_ep6_dat;
		d0 |= reg_usb_ep6_dat << 8;
	    signed short d1 = reg_usb_ep6_dat;
		d1 |= reg_usb_ep6_dat << 8;

		iso_out_buff[iso_out_w++ & (SPK_BUFFER_SIZE - 1)] = d0;
		iso_out_buff[iso_out_w++ & (SPK_BUFFER_SIZE - 1)] = d1;
	}
	usbhw_data_ep_ack(USB_EDP_SPEAKER);
}



int		num_iso_in = 0;
volatile int 	num_iso_out = 0;

_attribute_ram_code_sec_ void  usb_endpoint_irq_handler (void)
{
	/////////////////////////////////////
	// ISO IN
	/////////////////////////////////////
	if (usbhw_get_eps_irq()& FLD_USB_EDP7_IRQ)
	{
		usbhw_clr_eps_irq(FLD_USB_EDP7_IRQ);	//clear interrupt flag of endpoint 7
		/////// get MIC input data ///////////////////////////////
		audio_tx_data_to_usb(MIC_SAMPLING_RATE);
		num_iso_in++;
		if ((num_iso_in & 0x7f) == 0) gpio_toggle(LED2);
	}

	/////////////////////////////////////
	// ISO OUT
	/////////////////////////////////////
	if (usbhw_get_eps_irq()&FLD_USB_EDP6_IRQ)
	{
		usbhw_clr_eps_irq(FLD_USB_EDP6_IRQ);
		///////////// output to audio fifo out ////////////////
		audio_rx_data_from_usb();
		num_iso_out++;
		if ((num_iso_out & 0x7f) == 0) gpio_toggle(LED3);
	}

}
PLIC_ISR_REGISTER(usb_endpoint_irq_handler, IRQ_USB_ENDPOINT)

#if (MCU_CORE_B91)

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
	plic_interrupt_enable(IRQ_USB_ENDPOINT);		// enable usb endpoint interrupt

	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
	//3.enable global interrupt
	core_interrupt_enable();
	usbhw_set_irq_mask(USB_IRQ_RESET_MASK|USB_IRQ_SUSPEND_MASK);
	/*1  "AUDIO_ADC_16K_DAC_48K" - mic sampling=16K,spk sampling=48K;"AUDIO_16K"-mic sampling=16K,spk sampling=16K,should match the setting  in usb_default.h
	 *2  mic and spk  channel count should be the same (1 or 2 ) in usb_default.h
	 *3  channel count =1,MONO_BIT_16;channel count =2  STEREO_BIT_16*/
	audio_init(LINE_IN_TO_BUF_TO_LINE_OUT,MIC_SAMPLING_RATE,STEREO_BIT_16);
	audio_rx_dma_chain_init(MIC_DMA_RX_CHN,iso_in_buff,MIC_BUFFER_SIZE * 2);
	audio_tx_dma_chain_init (SPK_DMA_TX_CHN,iso_out_buff, SPK_BUFFER_SIZE * 2);


}
unsigned int t=0;
int temp=0;
void main_loop (void)
{

	usb_handle_irq();
	if(clock_time_exceed(t,2000)&&(num_iso_out))//2ms
	{
		if(num_iso_out==temp)
		{
			memset(iso_out_buff,0,SPK_BUFFER_SIZE * 2);//if host pause playback clear buff.
			num_iso_out=0;
		}

		gpio_toggle(LED4);
		temp=num_iso_out;
		t = stimer_get_tick()|1;

	}
}
#elif(MCU_CORE_B92)

#define MIC_FIFO_NUM   		    FIFO0
#define SPK_FIFO_NUM   		    FIFO1
#define MIC_DMA_TX_CHN   		DMA2
#define SPK_DMA_RX_CHN   		DMA3

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

	reg_usb_ep6_buf_addr = 0x40;		// 192 max
	reg_usb_ep7_buf_addr = 0x20;		// 32
	reg_usb_ep8_buf_addr = 0x00;
	reg_usb_ep_max_size = (192 >> 3);
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
	audio_codec_init();

#if(USB_MODE==INT)

	 audio_codec_stream0_input_t audio_codec_input = {
	.input_src = LINE_STREAM0_STEREO,
	.sample_rate = MIC_SAMPLING_RATE,
	.fifo_num = MIC_FIFO_NUM,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = MIC_DMA_RX_CHN,
	.data_buf = iso_in_buff,
	.data_buf_size = sizeof(iso_in_buff),
	};
	 audio_codec_output_t audio_codec_output = {
	.output_src = CODEC_DAC_STEREO,
	.sample_rate = SPK_SAMPLING_RATE,
	.fifo_num = SPK_FIFO_NUM,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = SPK_DMA_TX_CHN,
	.mode = HP_MODE,
	.data_buf = iso_out_buff,
	.data_buf_size = sizeof(iso_out_buff),
	};

	core_interrupt_enable();//enable global interrupt
	plic_interrupt_enable(IRQ_USB_ENDPOINT);		// enable usb endpoint interrupt
	usbhw_set_eps_irq_mask(FLD_USB_EDP7_IRQ);
	usbhw_set_eps_irq_mask(FLD_USB_EDP6_IRQ);
	/***mic***/
	audio_codec_stream0_input_init(&audio_codec_input);
	audio_rx_dma_chain_init(audio_codec_input.fifo_num,audio_codec_input.dma_num,(unsigned short*)audio_codec_input.data_buf,audio_codec_input.data_buf_size);
	/***spk***/
	audio_codec_stream_output_init(&audio_codec_output);
	audio_tx_dma_chain_init(audio_codec_output.fifo_num,audio_codec_output.dma_num,(unsigned short*)audio_codec_output.data_buf,audio_codec_output.data_buf_size);
	/***DMA enable***/
	audio_rx_dma_en(audio_codec_input.dma_num);
	audio_tx_dma_en(audio_codec_output.dma_num);
#else


#define AISO_TX_DMA_CHN            DMA4
#define AISO_RX_DMA_CHN            DMA5
	 audio_codec_stream0_input_t audio_codec_input = {
	.input_src = LINE_STREAM0_STEREO,
	.sample_rate = MIC_SAMPLING_RATE,
	.fifo_num = MIC_FIFO_NUM,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = MIC_DMA_RX_CHN,
	.data_buf = iso_in_buff,
	.data_buf_size = sizeof(iso_in_buff),
    };

	 audio_codec_output_t audio_codec_output = {
	.output_src = CODEC_DAC_STEREO,
	.sample_rate = SPK_SAMPLING_RATE,
	.fifo_num = SPK_FIFO_NUM,
	.data_width = CODEC_BIT_16_DATA,
	.dma_num = SPK_DMA_TX_CHN,
	.mode = HP_MODE,
	.data_buf = iso_out_buff,
	.data_buf_size = sizeof(iso_out_buff),
	};

	/***mic***/
	 audio_codec_init();
	 audio_codec_stream0_input_init(&audio_codec_input);
	 audio_data_fifo_output_path_sel(MIC_FIFO_NUM,USB_AISO_OUT_FIFO);
	 audio_rx_dma_chain_init(audio_codec_input.fifo_num,audio_codec_input.dma_num,(unsigned short*)audio_codec_input.data_buf,audio_codec_input.data_buf_size);
	 audio_tx_dma_chain_init(MIC_FIFO_NUM,AISO_TX_DMA_CHN,(unsigned short*)iso_in_buff,MIC_BUFFER_SIZE*2);
	 audio_rx_dma_en(audio_codec_input.dma_num);
	 audio_tx_dma_en(AISO_TX_DMA_CHN);
	 /***spk***/
	 audio_codec_stream_output_init(&audio_codec_output);
	 audio_data_fifo_input_path_sel(SPK_FIFO_NUM,USB_AISO_IN_FIFO);
	 audio_rx_dma_chain_init(SPK_FIFO_NUM,AISO_RX_DMA_CHN,(unsigned short*)iso_out_buff,MIC_BUFFER_SIZE*2);
	 audio_tx_dma_chain_init(audio_codec_output.fifo_num,audio_codec_output.dma_num,(unsigned short*)audio_codec_output.data_buf,audio_codec_output.data_buf_size);
	 audio_rx_dma_en(AISO_RX_DMA_CHN);
	 audio_tx_dma_en(audio_codec_output.dma_num);
#endif
}
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
}



#endif

#endif
