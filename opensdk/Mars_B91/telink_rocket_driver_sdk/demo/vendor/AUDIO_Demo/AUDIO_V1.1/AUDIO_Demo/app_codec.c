/********************************************************************************************************
 * @file    app_codec.c
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
#if (AUDIO_MODE <=DMA_IRQ_Test)
#include "app_sin_data.h"
#define    AUDIO_BUFF_SIZE  4096
signed short AUDIO_BUFF[AUDIO_BUFF_SIZE>>1] __attribute__((aligned(4)));
signed short AUDIO_BUFF1[AUDIO_BUFF_SIZE>>1] __attribute__((aligned(4)));

#if ((AUDIO_MODE== LINE_INPUT_TO_BUF_TO_LINEOUT)||(AUDIO_MODE== AMIC_INPUT_TO_BUF_TO_LINEOUT)||(AUDIO_MODE== DMIC0_INPUT_TO_BUF_TO_LINEOUT)||(AUDIO_MODE== DMIC1_INPUT_TO_BUF_TO_LINEOUT))

#define SAMPLE_RATE     AUDIO_16K
#define DATA_WIDTH      CODEC_BIT_16_DATA
#define FIFO_NUM        FIFO0
#define RX_DMA_CHN      DMA2
#define TX_DMA_CHN      DMA3
/*INPUT source select*/
#if  (AUDIO_MODE == LINE_INPUT_TO_BUF_TO_LINEOUT)
#define INPUT_SRC  LINE_STREAM0_STEREO

#elif(AUDIO_MODE == AMIC_INPUT_TO_BUF_TO_LINEOUT)
#define INPUT_SRC  AMIC_STREAM0_STEREO

#elif(AUDIO_MODE == DMIC0_INPUT_TO_BUF_TO_LINEOUT)
#define INPUT_SRC  DMIC_STREAM0_STEREO

#elif(AUDIO_MODE == DMIC1_INPUT_TO_BUF_TO_LINEOUT)
#define INPUT_SRC  DMIC_STREAM1_STEREO

#endif
/*OUTPUT source select :The output must be consistent with the number of the input channels,
 *   for example:INPUT_MONO ==> OUTPUT_MON0 or INPUT_STEREO ==> OUTPUT_STEREO*/
#define OUTPUT_SRC      CODEC_DAC_STEREO

#if (AUDIO_MODE != DMIC1_INPUT_TO_BUF_TO_LINEOUT)

    audio_codec_stream0_input_t audio_codec_input =
    {
		.input_src = INPUT_SRC,
		.sample_rate = SAMPLE_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = DATA_WIDTH,
		.dma_num = RX_DMA_CHN,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
    };

#else
	audio_codec_stream1_input_t audio_codec_input =
	{
		.input_src = INPUT_SRC,
		.sample_rate = SAMPLE_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = DATA_WIDTH,
		.dma_num = RX_DMA_CHN,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};
#endif

	 audio_codec_output_t audio_codec_output =
	 {
		.output_src = OUTPUT_SRC,
		.sample_rate = SAMPLE_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = DATA_WIDTH,
		.dma_num = TX_DMA_CHN,
		.mode = HP_MODE,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};

#endif

void user_init(void)
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);

	/****audio init ****/
	audio_codec_init();

#if ((AUDIO_MODE== LINE_INPUT_TO_BUF_TO_LINEOUT)||(AUDIO_MODE== AMIC_INPUT_TO_BUF_TO_LINEOUT)||(AUDIO_MODE== DMIC0_INPUT_TO_BUF_TO_LINEOUT)||(AUDIO_MODE== DMIC1_INPUT_TO_BUF_TO_LINEOUT))

#if (AUDIO_MODE != DMIC1_INPUT_TO_BUF_TO_LINEOUT)	/*sterm0 line/amic/dmic0 input init*/

	 audio_codec_stream0_input_init(&audio_codec_input);
#if (AUDIO_MODE==AMIC_INPUT_TO_BUF_TO_LINEOUT)
	 audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_18_DB);
#endif
#if (AUDIO_MODE==DMIC0_INPUT_TO_BUF_TO_LINEOUT)
	 audio_set_stream0_dmic_pin(GPIO_FC_PD5,GPIO_FC_PD4,GPIO_FC_PD3);
	 audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_30_DB);
#endif

#else /*sterm1 dmic1 input init*/
	 audio_set_stream1_dmic_pin(GPIO_FC_PD5,GPIO_FC_PD4,GPIO_FC_PD3);
	 audio_codec_stream1_input_init(&audio_codec_input);
	 audio_set_stream1_dig_gain(CODEC_IN_D_GAIN1_18_DB);

#endif
	 /****line output init****/
	 audio_codec_stream_output_init(&audio_codec_output);
	 /****rx tx dma init****/
	 audio_rx_dma_chain_init(audio_codec_input.fifo_num,audio_codec_input.dma_num,(unsigned short*)audio_codec_input.data_buf,audio_codec_input.data_buf_size);
	 audio_tx_dma_chain_init(audio_codec_output.fifo_num,audio_codec_output.dma_num,(unsigned short*)audio_codec_output.data_buf,audio_codec_output.data_buf_size);
	 /****audio starts run****/
	 audio_rx_dma_en(audio_codec_input.dma_num);
	 audio_tx_dma_en(audio_codec_output.dma_num);

#endif

#if (AUDIO_MODE==AMIC_AND_DMIC1_INPUT_TO_TWO_BUF)
#define AMIC_RX_DMA_CHN         DMA2
#define DMIC_RX_DMA_CHN         DMA3
	/**amic&dmic input to two fifo to two buff**/
	 audio_codec_stream0_input_t audio_codec_amic_input =
	 {
		.input_src = AMIC_STREAM0_STEREO,
		.sample_rate = AUDIO_16K,
		.fifo_num = FIFO0,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = AMIC_RX_DMA_CHN,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};
	 audio_codec_stream1_input_t audio_codec_dmic_input =
	 {
		.input_src	 = DMIC_STREAM1_STEREO,
		.sample_rate = AUDIO_16K,
		.fifo_num = FIFO1,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = DMIC_RX_DMA_CHN,
		.data_buf = AUDIO_BUFF1,
		.data_buf_size = sizeof(AUDIO_BUFF1),
	};

	/****amic input init ****/
	audio_codec_stream0_input_init(&audio_codec_amic_input);
	/****dmic1 input init****/
	audio_set_stream1_dmic_pin(GPIO_FC_PD5,GPIO_FC_PD4,GPIO_FC_PD3);
	audio_codec_stream1_input_init(&audio_codec_dmic_input);
	/****rx tx dma init****/
	audio_rx_dma_chain_init(audio_codec_amic_input.fifo_num,audio_codec_amic_input.dma_num,(unsigned short*)audio_codec_amic_input.data_buf,audio_codec_amic_input.data_buf_size);
	audio_rx_dma_chain_init(audio_codec_dmic_input.fifo_num,audio_codec_dmic_input.dma_num,(unsigned short*)audio_codec_dmic_input.data_buf,audio_codec_dmic_input.data_buf_size);
	/****audio starts run****/
	audio_rx_dma_en(audio_codec_amic_input.dma_num);
	audio_rx_dma_en(audio_codec_dmic_input.dma_num);

#elif (AUDIO_MODE==DMIC0_AND_DMIC1_INPUT_TO_ONE_BUF)
#define   SAMPLE_RATE     AUDIO_16K
#define   FIFO_NUM            FIFO0
#define   RX_DMA_CHN          DMA2

	 /****dmic0&dmic1 input to one fifo to one buff
	  * The data of stream0 and stream1 are stored in one buff,only stereo 16 bits of data bit width are supported,[stream1 data(the first 4bytes),stream0 data(the second 4bytes)]
	  *                                                                                                              {dmic1_r[15:0], dmic1_l[15:0],dmic0_r[15:0], dmic1_l[15:0]}
	  * */
	 audio_codec_stream0_input_t audio_codec_dmic0_input =
	 {
		.input_src	 = DMIC_STREAM0_STEREO,
		.sample_rate = SAMPLE_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,//must 16 bit
		.dma_num = RX_DMA_CHN,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};
	 audio_codec_stream1_input_t audio_codec_dmic1_input =
	 {
		.input_src	 = DMIC_STREAM0_STREAM1_STEREO,
		.sample_rate = SAMPLE_RATE,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,//must 16 bit
		.dma_num = RX_DMA_CHN,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};
	 /****dmic0,dmic1 pin init ****/
	audio_set_stream0_dmic_pin(GPIO_FC_PD2,GPIO_FC_PD7,GPIO_FC_PD6);
	audio_set_stream1_dmic_pin(GPIO_FC_PD5,GPIO_FC_PD4,GPIO_FC_PD3);
	/****dmic0,dmic1 input ****/
	audio_codec_stream0_input_init(&audio_codec_dmic0_input);
	audio_codec_stream1_input_init(&audio_codec_dmic1_input);
	audio_set_stream0_dig_gain1(CODEC_IN_D_GAIN1_30_DB);
	audio_set_stream1_dig_gain(CODEC_IN_D_GAIN1_30_DB);
	/****rx dma init****/
	audio_rx_dma_chain_init(FIFO_NUM,RX_DMA_CHN,(unsigned short*)AUDIO_BUFF,sizeof(AUDIO_BUFF));
	/****audio starts run****/
	audio_rx_dma_en(RX_DMA_CHN);

#elif(AUDIO_MODE==BUFFER_TO_LINEOUT)
#define TX_DMA_CHN      DMA3
	 audio_codec_output_t audio_codec_output =
	 {
		.output_src = CODEC_DAC_STEREO,
		.sample_rate = AUDIO_48K,
		.fifo_num = FIFO0,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = TX_DMA_CHN,
		.mode = HP_MODE,
		.data_buf = sin_48k_stereo,
		.data_buf_size = sizeof(sin_48k_stereo),
	};
	audio_codec_stream_output_init(&audio_codec_output);
	audio_tx_dma_chain_init(audio_codec_output.fifo_num,audio_codec_output.dma_num,(unsigned short*)audio_codec_output.data_buf,audio_codec_output.data_buf_size);
	audio_tx_dma_en(TX_DMA_CHN);
#elif(AUDIO_MODE==DMA_IRQ_Test)
#define FIFO_NUM            FIFO0
#define RX_DMA_CHN          DMA2
#define TX_DMA_CHN          DMA3
	 audio_codec_stream0_input_t audio_codec_amic_input =
	 {
		.input_src = AMIC_STREAM0_STEREO,    //The output must be consistent with the number of the input channels, For example:INPUT_MONO ==> OUTPUT_MON0 or INPUT_STEREO ==> OUTPUT_STEREO
		.sample_rate = AUDIO_16K,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = RX_DMA_CHN,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};

	 audio_codec_output_t audio_codec_output =
	 {
		.output_src = CODEC_DAC_STEREO,   //The output must be consistent with the number of the input channels, For example:INPUT_MONO ==> OUTPUT_MON0 or INPUT_STEREO ==> OUTPUT_STEREO
		.sample_rate = AUDIO_16K,
		.fifo_num = FIFO_NUM,
		.data_width = CODEC_BIT_16_DATA,
		.dma_num = TX_DMA_CHN,
		.mode = HP_MODE,
		.data_buf = AUDIO_BUFF,
		.data_buf_size = sizeof(AUDIO_BUFF),
	};
	 /****dma interrupt init****/
	core_interrupt_enable();
	plic_interrupt_enable(IRQ_DMA);
	dma_set_irq_mask(RX_DMA_CHN,TC_MASK);
	dma_set_irq_mask(TX_DMA_CHN,TC_MASK);
	/****amic input init****/
	audio_codec_stream0_input_init(&audio_codec_amic_input);
	/****line output init***/
	audio_codec_stream_output_init(&audio_codec_output);
	/****rx tx dma init****/
	audio_rx_dma_chain_init(audio_codec_amic_input.fifo_num,audio_codec_amic_input.dma_num,(unsigned short*)audio_codec_amic_input.data_buf,audio_codec_amic_input.data_buf_size);
	audio_tx_dma_chain_init(audio_codec_output.fifo_num,audio_codec_output.dma_num,(unsigned short*)audio_codec_output.data_buf,audio_codec_output.data_buf_size);
	/****audio starts run****/
	audio_rx_dma_en(audio_codec_amic_input.dma_num);
	audio_tx_dma_en(audio_codec_output.dma_num);
#endif

}
#if(AUDIO_MODE==DMA_IRQ_Test)
volatile unsigned int irq_cnt[2];
volatile unsigned int  dma_irq_ptr_test[4];

_attribute_ram_code_sec_noinline_ void dma_irq_handler(void)
{
	if(dma_get_tc_irq_status(BIT(RX_DMA_CHN)))
	{
		dma_clr_tc_irq_status(BIT(RX_DMA_CHN));
		dma_irq_ptr_test[0]=audio_get_rx_dma_wptr(RX_DMA_CHN)-(unsigned int )AUDIO_BUFF;//dma_irq_ptr_test[0]=dma_irq_ptr_test[1]
		dma_irq_ptr_test[1]=audio_get_rx_wptr(FIFO_NUM);
		irq_cnt[0]++;
	}

	if(dma_get_tc_irq_status(BIT(TX_DMA_CHN)))
	{
		dma_clr_tc_irq_status(BIT(TX_DMA_CHN));
		dma_irq_ptr_test[2]=audio_get_tx_dma_rptr(TX_DMA_CHN)-(unsigned int )AUDIO_BUFF;//dma_irq_ptr_test[2]=dma_irq_ptr_test[3]
		dma_irq_ptr_test[3]=audio_get_tx_rptr(FIFO_NUM);
		irq_cnt[1]++;
	}
}
PLIC_ISR_REGISTER(dma_irq_handler, IRQ_DMA)
#endif

void main_loop (void)
{
	gpio_toggle(LED1);
	delay_ms(1000);

}
#endif

